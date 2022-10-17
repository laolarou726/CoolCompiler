//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_CASE_H
#define COOLCOMPILER_CASE_H

#include <vector>
#include "Expression.h"
#include "CaseAction.h"

namespace CoolCompiler {

    class Case : public Expression {
    private:
        Expression* expression;
        std::vector<CaseAction*> actions;
    public:
        Case(Expression *expression, const std::vector<CaseAction*> &actions);
        [[nodiscard]] Expression* getExpression() const;
        [[nodiscard]] std::vector<CaseAction*> getActions() const;

        void print(int depth) override{
            printTab(depth);
            std::cout << "CASE [" << std::endl;

            expression->print(depth + 1);

            std::cout << "]" << std::endl;

            for(CaseAction *ca : actions)
                ca->print(depth + 1);
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_CASE_H
