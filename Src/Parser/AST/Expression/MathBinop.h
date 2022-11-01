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

        std::string intOpCheck(SemanticAnalyzer* analyzer, const std::string &symbol);
        std::string typeCheck(SemanticAnalyzer* analyzer) override;

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
