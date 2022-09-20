//
// Created by luoli on 2022/9/18.
//

#include "Not.h"

namespace CoolCompiler {
    Not::Not(const Expression &expression) : Expression("not") {
        this->expression = expression;
    }

    Expression Not::getExpression() const {
        return expression;
    }
} // CoolCompiler