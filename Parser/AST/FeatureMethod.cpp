//
// Created by luoli on 2022/9/18.
//

#include "FeatureMethod.h"

namespace CoolCompiler {

    CoolCompiler::FeatureMethod::FeatureMethod(const std::string &name, const std::string &returnType,
                                               const Expression &expression,
                                               const std::vector<Formal> &formalArguments) : AST("feature_method") {
        this->name = name;
        this->returnType = returnType;
        this->expression = expression;
        this->formalArguments = formalArguments;
    }

    std::string CoolCompiler::FeatureMethod::getname() const {
        return name;
    }

    std::string CoolCompiler::FeatureMethod::getReturnType() const {
        return returnType;
    }

    CoolCompiler::Expression CoolCompiler::FeatureMethod::getExpression() const {
        return expression;
    }

    std::vector<Formal> CoolCompiler::FeatureMethod::getFormalArguments() const {
        return formalArguments;
    }

}
