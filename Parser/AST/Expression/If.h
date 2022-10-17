//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_IF_H
#define COOLCOMPILER_IF_H

#include "Expression.h"

namespace CoolCompiler {

    class If : public Expression {
    private:
        Expression* condition;
        AST* conditionTrue;
        AST* conditionFalse;
    public:
        If(Expression* condition, AST* conditionTrue, AST* conditionFalse);
        [[nodiscard]] Expression* getCondition() const;
        [[nodiscard]] AST* getConditionTrue() const;
        [[nodiscard]] AST* getConditionFalse() const;

        void print(int depth) override{
            printTab(depth);
            std::cout << "IF [" << std::endl;

            printTab(depth + 1);
            std::cout << "CONDITION [" << std::endl;

            condition->print(depth + 2);

            printTab(depth + 1);
            std::cout << "]" << std::endl;

            printTab(depth + 1);
            std::cout << "TRUE [" << std::endl;

            conditionTrue->print(depth + 2);

            printTab(depth + 1);
            std::cout << "]" << std::endl;

            printTab(depth + 1);
            std::cout << "FALSE [" << std::endl;

            conditionFalse->print(depth + 2);

            printTab(depth + 1);
            std::cout << "]" << std::endl;

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_IF_H
