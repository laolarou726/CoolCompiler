//
// Created by luoli on 2022/9/18.
//

#include "FeatureAttribute.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "fmt/format.h"

namespace CoolCompiler {
    FeatureAttribute::FeatureAttribute(const std::string &name, const std::string &type) : FeatureBase("feature_attr"){
        this->name = name;
        this->type = type;
        this->init = nullptr;
    }

    FeatureAttribute::FeatureAttribute(const std::string &name, const std::string &type,
                                       Expression* init) : FeatureBase("feature_attr") {
        this->name = name;
        this->type = type;
        this->init = init;
    }

    std::string FeatureAttribute::getName() const {
        return name;
    }

    std::string FeatureAttribute::getType() const {
        return type;
    }

    Expression* FeatureAttribute::getInit() const {
        return init;
    }

    std::string FeatureAttribute::typeCheck(CoolCompiler::SemanticAnalyzer *analyzer) {
        if(init == nullptr)
            return type;

        std::string initType = init->typeCheck(analyzer);
        initType = initType == "SELF_TYPE" ? analyzer->getCurrentClassName() : initType;

        if(name == "Self"){
            analyzer->fail("'self' cannot be the name of an attribute.");
            return type;
        }

        if(!analyzer->isTypeDefined(initType)){
            std::string message = fmt::format("The attribute [{}] is defined as <{}>, but type <{}> is undefined.",
                                              name, type, type);
            analyzer->fail(message);

            return type;
        }

        bool isInitTypeMatchDefinedType = analyzer->isSubtype(initType, type);

        if(!isInitTypeMatchDefinedType){
            std::string message = fmt::format("The attribute [{}]  is defined as <{}> but is initialized with <{}>.",
                                              name, type, initType);
            analyzer->fail(message);
        }

        return type;
    }

} // CoolCompiler