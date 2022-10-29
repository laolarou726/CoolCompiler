//
// Created by luoli on 2022/9/18.
//

#include "While.h"

namespace CoolCompiler {
    While::While(Expression* condition, Expression* body) : Expression("while") {
        this->condition = condition;
        this->body = body;
    }

    Expression* While::getCondition() const {
        return condition;
    }

    Expression* While::getBody() const {
        return body;
    }
} // CoolCompiler