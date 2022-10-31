//
// Created by luoli on 2022/9/18.
//

#include "MathBinop.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    MathBinop::MathBinop(const std::string &token, TokenType operation, Expression *expressionLeft,
                         Expression *expressionRight) : Expression("math_binop") {
        this->token = token;
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

    std::string MathBinop::eqCheck(SemanticAnalyzer *analyzer) {
        std::string leftExprType = expressionLeft->typeCheck(analyzer);
        std::string rightExprType = expressionRight->typeCheck(analyzer);
        bool isLeftTypePrimitive = analyzer->isPrimitive(leftExprType);
        bool isRightTypePrimitive = analyzer->isPrimitive(rightExprType);

        if((isLeftTypePrimitive && isRightTypePrimitive) && leftExprType!= rightExprType){
            analyzer->fail("Illegal comparison with a basic type.");
        }

        return "Bool";
    }

    std::string MathBinop::boolOpCheck(SemanticAnalyzer *analyzer, const std::string &symbol) {
        std::string leftExprType = expressionLeft->typeCheck(analyzer);
        std::string rightExprType = expressionRight->typeCheck(analyzer);

        if(leftExprType == "Int" && rightExprType == "Int")
            return "Bool";

        std::string message = fmt::format("{}: Expected both arguments of operator {} to be of type Int but got arguments of types <{}> and <{}>.",
                                          "Math_Binop", symbol, leftExprType, rightExprType);
        analyzer->fail(message);

        return "Object";
    }

    std::string MathBinop::intOpCheck(SemanticAnalyzer *analyzer, const std::string &symbol) {
        std::string leftExprType = expressionLeft->typeCheck(analyzer);
        std::string rightExprType = expressionRight->typeCheck(analyzer);

        if(leftExprType == "Int" && rightExprType == "Int")
            return "Int";

        std::string message = fmt::format("{}: Expected both arguments of operator {} to be of type Int but got arguments of types <{}> and <{}>.",
                                          "Math_Binop", symbol, leftExprType, rightExprType);
        analyzer->fail(message);

        return "Object";
    }

    std::string MathBinop::typeCheck(SemanticAnalyzer *analyzer) {
        switch (operation) {
            case LTOE:
                return boolOpCheck(analyzer, "<=");
            case GTOE:
                return boolOpCheck(analyzer, ">=");
            case EQ:
                return eqCheck(analyzer);
            case PLUS:
                return intOpCheck(analyzer, "+");
            case MINUS:
                return intOpCheck(analyzer, "-");
            case STAR:
                return intOpCheck(analyzer, "*");
            case SLASH:
                return intOpCheck(analyzer, "/");
        }
    }
} // CoolCompiler