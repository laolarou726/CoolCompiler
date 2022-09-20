//
// Created by luoli on 2022/9/18.
//

#include "Comparison.h"

namespace CoolCompiler {
    Comparison::Comparison(const std::string &operation, const Expression &expressionLeft,
                           const Expression &expressionRight) : Expression("comparison") {

    }

    std::string Comparison::getOperation() const {
        return operation;
    }

    Expression Comparison::getExpressionLeft() const {
        return expressionLeft;
    }

    Expression Comparison::getExpressionRight() const {
        return expressionRight;
    }
} // CoolCompiler