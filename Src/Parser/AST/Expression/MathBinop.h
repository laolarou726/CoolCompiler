//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_MATHBINOP_H
#define COOLCOMPILER_MATHBINOP_H

#include "Expression.h"
#include "../../../Lexer/Token.h"
#include "fmt/format.h"

namespace CoolCompiler {

    class MathBinop : public Expression {
    private:
        std::string token;
        TokenType operation;
        Expression* expressionLeft;
        Expression* expressionRight;
    public:
        MathBinop(const std::string &token, TokenType operation, Expression *expressionLeft, Expression *expressionRight);
        [[nodiscard]] TokenType getOperation() const;
        [[nodiscard]] Expression* getExpressionLeft() const;
        [[nodiscard]] Expression* getExpressionRight() const;

        std::string eqCheck(SemanticAnalyzer* analyzer){
            std::string leftExprType = expressionLeft->typeCheck(analyzer);
            std::string rightExprType = expressionRight->typeCheck(analyzer);
            bool isLeftTypePrimitive = analyzer->isPrimitive(leftExprType);
            bool isRightTypePrimitive = analyzer->isPrimitive(rightExprType);

            if((isLeftTypePrimitive && isRightTypePrimitive) && leftExprType!= rightExprType){
                analyzer->fail("Illegal comparison with a basic type.");
            }

            return "Bool";
        }

        std::string boolOpCheck(SemanticAnalyzer* analyzer, const std::string &symbol){
            std::string leftExprType = expressionLeft->typeCheck(analyzer);
            std::string rightExprType = expressionRight->typeCheck(analyzer);

            if(leftExprType == "Int" && rightExprType == "Int")
                return "Bool";

            std::string message = fmt::format("{}: Expected both arguments of operator {} to be of type Int but got arguments of types <{}> and <{}>.",
                                              "Math_Binop", symbol, leftExprType, rightExprType);
            analyzer->fail(message);

            return "Object";
        }

        std::string intOpCheck(SemanticAnalyzer* analyzer, const std::string &symbol){
            std::string leftExprType = expressionLeft->typeCheck(analyzer);
            std::string rightExprType = expressionRight->typeCheck(analyzer);

            if(leftExprType == "Int" && rightExprType == "Int")
                return "Int";

            std::string message = fmt::format("{}: Expected both arguments of operator {} to be of type Int but got arguments of types <{}> and <{}>.",
                                              "Math_Binop", symbol, leftExprType, rightExprType);
            analyzer->fail(message);

            return "Object";
        }

        std::string typeCheck(SemanticAnalyzer* analyzer) override{
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

        void print(int depth) override{
            printTab(depth);
            std::cout << "MATH_BI[" << std::endl;

            expressionLeft->print(depth + 1);

            printTab(depth + 1);
            std::cout << token << std::endl;

            expressionRight->print(depth + 1);

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_MATHBINOP_H
