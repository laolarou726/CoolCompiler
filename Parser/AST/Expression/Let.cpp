//
// Created by luoli on 2022/9/18.
//

#include "Let.h"

namespace CoolCompiler {
    Let::Let(const std::vector<InnerLet> &parameters, const Expression &expression) : Expression("let") {
        this->parameters = parameters;
        this->expression = expression;
    }

    std::vector<InnerLet> Let::getParameters() const {
        return parameters;
    }

    Expression Let::getExpression() const {
        return expression;
    }
} // CoolCompiler