//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_CASE_H
#define COOLCOMPILER_CASE_H

#include <vector>
#include "Expression.h"

namespace CoolCompiler {

    class Case : Expression {
    private:
        Expression expression;
        std::vector<AST> actions;
    public:
        Case(const Expression &expression, const std::vector<AST> &actions);
        [[nodiscard]] Expression getExpression() const;
        [[nodiscard]] std::vector<AST> getActions() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_CASE_H
