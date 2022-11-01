//
// Created by luoli on 2022/9/18.
//

#include <set>
#include "FeatureMethod.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "fmt/format.h"

namespace CoolCompiler {

    FeatureMethod::FeatureMethod(const std::string &name, const std::string &returnType, Expression *expression)
    : FeatureBase("feature_method") {
        this->name = name;
        this->returnType = returnType;
        this->expression = expression;
    }

    CoolCompiler::FeatureMethod::FeatureMethod(const std::string &name,
                                               const std::string &returnType,
                                               Expression* expression,
                                               const std::vector<Formal*> &formalArguments)
                                               : FeatureBase("feature_method") {
        this->name = name;
        this->returnType = returnType;
        this->expression = expression;
        this->formalArguments = formalArguments;
    }

    std::string CoolCompiler::FeatureMethod::getName() const {
        return name;
    }

    std::string CoolCompiler::FeatureMethod::getReturnType() const {
        return returnType;
    }

    Expression* CoolCompiler::FeatureMethod::getExpression() const {
        return expression;
    }

    std::vector<Formal*> CoolCompiler::FeatureMethod::getFormalArguments() const {
        return formalArguments;
    }

    std::string FeatureMethod::typeCheck(CoolCompiler::SemanticAnalyzer *analyzer) {
        auto* objectsTable = analyzer->getObjectsTable();
        std::set<std::string> definedArgs;

        objectsTable->enter();

        for(auto* formal : formalArguments){
            std::string formalName = formal->getName();
            std::string formalType = formal->getType();

            if(formalName == "Self")
                analyzer->fail("'self' cannot be the name of a method argument.");

            if(definedArgs.find(formalName) != definedArgs.end()){
                std::string message = fmt::format("The argument [{}] in the signature of method [{}] has already been defined.",
                                                  formalName, name);
                analyzer->fail(message);
            }
            else{
                definedArgs.emplace(formalName);
            }

            if(!analyzer->isTypeDefined(formalType)){
                std::string message = fmt::format("The argument [{}] in the signature of method [{}] has undefined type <{}>.",
                                                  formalName, name, formalType);
                analyzer->fail(message);
            }
            else{
                objectsTable->add(formalName, new std::string(formalType));
            }
        }

        std::string expectedReturnType = returnType == "SELF_TYPE" ? analyzer->getCurrentClassName() : returnType;
        std::string actualReturnType = expression->typeCheck(analyzer);
        actualReturnType = actualReturnType == "SELF_TYPE" ? analyzer->getCurrentClassName() : actualReturnType;

        if(!analyzer->isSubtype(actualReturnType, expectedReturnType)){
            std::string message = fmt::format("Inferred return type <{}> of the method [{}] is not compatible with declared return type <{}>.",
                                              actualReturnType, name, returnType);
            analyzer->fail(message);
        }

        objectsTable->exit();

        return returnType;
    }

}
