//
// Created by luoli on 2022/9/18.
//

#include "MathBinop.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"

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
            case PLUS:
                return intOpCheck(analyzer, "+");
            case MINUS:
                return intOpCheck(analyzer, "-");
            case STAR:
                return intOpCheck(analyzer, "*");
            case SLASH:
                return intOpCheck(analyzer, "/");
        }

        return nullptr;
    }

    llvm::Value *MathBinop::visit(CoolCompiler::CodeGenerator *generator) {
        llvm::Value* leftExprValue = expressionLeft->visit(generator);
        llvm::Value* rightExprValue = expressionRight->visit(generator);
        auto* builder = generator->getBuilder();

        switch (operation) {
            case PLUS:
                return builder->CreateAdd(leftExprValue, rightExprValue);
            case MINUS:
                return builder->CreateSub(leftExprValue, rightExprValue);
            case STAR:
                return builder->CreateMul(leftExprValue, rightExprValue);
            case SLASH:
                return builder->CreateSDiv(leftExprValue, rightExprValue);
        }

        return nullptr;
    }
} // CoolCompiler