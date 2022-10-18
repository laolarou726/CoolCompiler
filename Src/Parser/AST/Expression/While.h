//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_WHILE_H
#define COOLCOMPILER_WHILE_H

#include "Expression.h"

namespace CoolCompiler {

    class While : public Expression {
    private:
        Expression* condition;
        AST* body;
    public:
        While(Expression* condition, AST* body);
        [[nodiscard]] Expression* getCondition() const;
        [[nodiscard]] AST* getBody() const;

        void print(int depth) override{
            printTab(depth);
            std::cout << "WHILE [" << std::endl;

            printTab(depth + 1);
            std::cout << "WHEN [" << std::endl;

            condition->print(depth + 2);

            printTab(depth + 1);
            std::cout << "]" << std::endl;

            printTab(depth + 1);
            std::cout << "DO [" << std::endl;

            body->print(depth + 2);

            printTab(depth + 1);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_WHILE_H
