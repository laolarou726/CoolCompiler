//
// Created by luoli on 2022/9/18.
//

#include "Tilde.h"

namespace CoolCompiler {
    Tilde::Tilde(const Expression &expression) : Expression("tilde") {
        this->expression = expression;
    }

    Expression Tilde::getExpression() const {
        return expression;
    }
} // CoolCompiler