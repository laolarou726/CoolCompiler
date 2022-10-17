//
// Created by luoli on 2022/9/18.
//

#include "FeatureAttribute.h"

namespace CoolCompiler {
    FeatureAttribute::FeatureAttribute(const std::string &name, const std::string &type) {
        this->name = name;
        this->type = type;
        this->init = nullptr;
    }

    FeatureAttribute::FeatureAttribute(const std::string &name, const std::string &type,
                                       Expression* init) : AST("feature_attr") {
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

} // CoolCompiler