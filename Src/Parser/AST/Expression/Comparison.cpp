//
// Created by luoli on 2022/9/18.
//

#include "Comparison.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    Comparison::Comparison(const std::string &token, TokenType operation, Expression *expressionLeft, Expression *expressionRight)
    : Expression("comparison") {
        this->token = token;
        this->operation = operation;
        this->expressionLeft = expressionLeft;
        this->expressionRight = expressionRight;
    }

    TokenType Comparison::getOperation() const {
        return operation;
    }

    Expression* Comparison::getExpressionLeft() const {
        return expressionLeft;
    }

    Expression* Comparison::getExpressionRight() const {
        return expressionRight;
    }

    std::string Comparison::eqCheck(SemanticAnalyzer *analyzer) {
        std::string leftExprType = expressionLeft->typeCheck(analyzer);
        std::string rightExprType = expressionRight->typeCheck(analyzer);
        bool isLeftTypePrimitive = analyzer->isPrimitive(leftExprType);
        bool isRightTypePrimitive = analyzer->isPrimitive(rightExprType);

        if((isLeftTypePrimitive && isRightTypePrimitive) && leftExprType!= rightExprType){
            analyzer->fail("Illegal comparison with a basic type.");
        }

        return "Bool";
    }

    std::string Comparison::boolOpCheck(SemanticAnalyzer *analyzer, const std::string &symbol) {
        std::string leftExprType = expressionLeft->typeCheck(analyzer);
        std::string rightExprType = expressionRight->typeCheck(analyzer);

        if(leftExprType == "Int" && rightExprType == "Int")
            return "Bool";

        std::string message = fmt::format("{}: Expected both arguments of operator {} to be of type Int but got arguments of types <{}> and <{}>.",
                                          "Comparison", symbol, leftExprType, rightExprType);
        analyzer->fail(message);

        return "Object";
    }

    std::string Comparison::typeCheck(SemanticAnalyzer *analyzer) {
        switch (operation) {
            case LT:
                return boolOpCheck(analyzer, "<");
            case GT:
                return boolOpCheck(analyzer, ">");
            case LTOE:
                return boolOpCheck(analyzer, "<=");
            case GTOE:
                return boolOpCheck(analyzer, ">=");
            case EQ:
                return eqCheck(analyzer);
        }
    }
} // CoolCompiler