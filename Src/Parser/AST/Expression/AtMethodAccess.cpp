//
// Created by luoli on 2022/9/18.
//

#include "AtMethodAccess.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    AtMethodAccess::AtMethodAccess(Expression* instance, const std::string &type, const std::string &method,
                                   const std::vector<Expression*> &arguments) : Expression("at_method_access") {
        this->instance = instance;
        this->type = type;
        this->method = method;
        this->arguments = arguments;
    }

    Expression* AtMethodAccess::getInstance() const {
        return instance;
    }

    std::string AtMethodAccess::getType() const {
        return type;
    }

    std::string AtMethodAccess::getMethod() const {
        return method;
    }

    std::vector<Expression*> AtMethodAccess::getArguments() const {
        return arguments;
    }

    std::string AtMethodAccess::typeCheck(SemanticAnalyzer *analyzer) {
        std::string exprType = instance->typeCheck(analyzer);

        if(type != "SELF_TYPE" && !analyzer->isTypeDefined(type)){
            std::string message = fmt::format("Static dispatch on undefined class [{}].",
                                              type);
            analyzer->fail(message);

            return "Object";
        }

        if(exprType != "SELF_TYPE" && !analyzer->isTypeDefined(exprType))
            return "Object";

        exprType = exprType == "SELF_TYPE" ? analyzer->getCurrentClassName() : exprType;

        if(!analyzer->isSubtype(exprType, type)){
            std::string message = fmt::format("Expression type <{}> is not compatible with declared static dispatch type <{}>.",
                                               exprType, type);
            analyzer->fail(message);
        }

        auto* methodDef = analyzer->lookupMethod(type, method);

        if(methodDef == nullptr){
            std::string message = fmt::format("Dispatch to undefined method [{}].",
                                              method);
            analyzer->fail(message);

            return "Object";
        }

        std::string returnType = methodDef->getReturnType();
        auto formals = methodDef->getFormalArguments();
        auto declaredArgsCount = arguments.size();
        auto actualArgsCount = formals.size();

        if(declaredArgsCount != actualArgsCount){
            std::string message = fmt::format("In the dispatch to method [{}], given number of arguments ({}) differs from the declared method's number of arguments ({}).",
                                              method, actualArgsCount, declaredArgsCount);
            analyzer->fail(message);
        }

        int index = 0;
        bool isDispatchValid = true;

        while(index < arguments.size() && index < formals.size()){
            std::string declaredArgType = formals[index]->getType();
            declaredArgType = declaredArgType == "SELF_TYPE" ? analyzer->getCurrentClassName() : declaredArgType;

            std::string actualArgType = arguments[index]->typeCheck(analyzer);
            actualArgType = actualArgType == "SELF_TYPE" ? analyzer->getCurrentClassName() : actualArgType;

            if(!analyzer->isSubtype(actualArgType, declaredArgType)){
                isDispatchValid = false;

                std::string message = fmt::format("In the dispatch of the method [{}], type <{}> of provided argument [{}] is not compatible with the corresponding signature type <{}>.",
                                                  method, actualArgType, formals[index]->getName(), declaredArgType);
                analyzer->fail(message);
            }

            index++;
        }

        if(!isDispatchValid)
            return "Object";

        return returnType == "SELF_TYPE" ? exprType : returnType;
    }
} // CoolCompiler