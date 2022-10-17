//
// Created by luoli on 2022/9/18.
//

#include "Comparison.h"

namespace CoolCompiler {
    Comparison::Comparison(const std::string &operation, Expression *expressionLeft, Expression *expressionRight)
    : Expression("comparison") {
        this->operation = operation;
        this->expressionLeft = expressionLeft;
        this->expressionRight = expressionRight;
    }

    std::string Comparison::getOperation() const {
        return operation;
    }

    Expression* Comparison::getExpressionLeft() const {
        return expressionLeft;
    }

    Expression* Comparison::getExpressionRight() const {
        return expressionRight;
    }
} // CoolCompiler