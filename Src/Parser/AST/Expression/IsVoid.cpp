//
// Created by luoli on 2022/9/18.
//

#include "IsVoid.h"

namespace CoolCompiler {
    IsVoid::IsVoid(Expression *expression) : Expression("is_void") {
        this->expression = expression;
    }

    Expression* IsVoid::getExpression() const {
        return expression;
    }
} // CoolCompiler