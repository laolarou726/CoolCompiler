//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_COMPARISON_H
#define COOLCOMPILER_COMPARISON_H

#include "Expression.h"

namespace CoolCompiler {

    class Comparison : Expression {
    private:
        std::string operation;
        Expression *expressionLeft;
        Expression *expressionRight;
    public:
        Comparison(const std::string &operation, Expression *expressionLeft, Expression *expressionRight);
        [[nodiscard]] std::string getOperation() const;
        [[nodiscard]] Expression* getExpressionLeft() const;
        [[nodiscard]] Expression* getExpressionRight() const;

        void print(int depth) override{
            printTab(depth);
            std::cout << "COMPARE [" << std::endl;

            expressionLeft->print(depth + 1);

            std::cout << operation << std::endl;

            expressionRight->print(depth + 1);

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_COMPARISON_H
