//
// Created by luoli on 2022/9/18.
//

#include "MathBinop.h"

namespace CoolCompiler {
    MathBinop::MathBinop(TokenType operation, Expression *expressionLeft,
                         Expression *expressionRight) : Expression("math_binop") {
        this->operation = operation;
        this->expressionLeft = expressionLeft;
        this->expressionRight = expressionRight;
    }

    TokenType MathBinop::getOperation() const {
        return operation;
    }

    Expression* MathBinop::getExpressionLeft() const {
        return expressionLeft;
    }

    Expression* MathBinop::getExpressionRight() const {
        return expressionRight;
    }
} // CoolCompiler