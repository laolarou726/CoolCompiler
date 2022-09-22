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
        Expression expression;
        std::vector<CaseAction> actions;
    public:
        Case(const Expression &expression, const std::vector<CaseAction> &actions);
        [[nodiscard]] Expression getExpression() const;
        [[nodiscard]] std::vector<CaseAction> getActions() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_CASE_H
