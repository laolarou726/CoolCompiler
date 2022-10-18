//
// Created by luoli on 2022/9/18.
//

#include "Assignment.h"

namespace CoolCompiler {
    Assignment::Assignment(Id* id, Expression* expression) : Expression("assignment")  {
        this->id = id;
        this->expression = expression;
    }

    Id* Assignment::getId() const {
        return id;
    }

    Expression* Assignment::getExpression() const {
        return expression;
    }
} // CoolCompiler