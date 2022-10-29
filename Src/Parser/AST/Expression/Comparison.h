//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_COMPARISON_H
#define COOLCOMPILER_COMPARISON_H

#include "Expression.h"
#include "fmt/format.h"

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

        std::string typeCheck(SemanticAnalyzer* analyzer) override{
            std::string leftExprType = expressionLeft->typeCheck(analyzer);
            std::string rightExprType = expressionRight->typeCheck(analyzer);

            if(leftExprType == "Int" && rightExprType == "Int")
                return "Bool";

            std::string message = fmt::format("{}: Expected both arguments of operator = to be of type Int but got arguments of types <{}> and <{}>.",
                                              "Comparison", leftExprType, rightExprType);
            analyzer->fail(message);

            return "Object";
        }

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
