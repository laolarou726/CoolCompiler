//
// Created by luoli on 2022/9/18.
//

#include "Comparison.h"
#include "../../../Semantic/SemanticAnalyzer.h"

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

    std::string Comparison::typeCheck(SemanticAnalyzer *analyzer) {
        std::string leftExprType = expressionLeft->typeCheck(analyzer);
        std::string rightExprType = expressionRight->typeCheck(analyzer);

        if(leftExprType == "Int" && rightExprType == "Int")
            return "Bool";

        std::string message = fmt::format("{}: Expected both arguments of operator = to be of type Int but got arguments of types <{}> and <{}>.",
                                          "Comparison", leftExprType, rightExprType);
        analyzer->fail(message);

        return "Object";
    }
} // CoolCompiler