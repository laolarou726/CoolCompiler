//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_LET_H
#define COOLCOMPILER_LET_H

#include <vector>
#include "Expression.h"

namespace CoolCompiler {

    class Let : Expression {
    private:
        std::vector<AST> parameters;
        Expression expression;
    public:
        Let(const std::vector<AST> &parameters, const Expression &expression);
        [[nodiscard]] std::vector<AST> getParameters() const;
        [[nodiscard]] Expression getExpression() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_LET_H
