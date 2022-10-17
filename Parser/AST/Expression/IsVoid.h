//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_ISVOID_H
#define COOLCOMPILER_ISVOID_H

#include "Expression.h"

namespace CoolCompiler {

    class IsVoid : public Expression {
    private:
        Expression* expression;
    public:
        explicit IsVoid(Expression *expression);
        [[nodiscard]] Expression* getExpression() const;

        void print(int depth) override{
            printTab(depth);
            std::cout << "IS_VOID [" << std::endl;

            expression->print(depth + 1);

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_ISVOID_H
