//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_WHILE_H
#define COOLCOMPILER_WHILE_H

#include "Expression.h"

namespace CoolCompiler {

    class While : Expression {
    private:
        Expression condition;
        AST body;
    public:
        While(const Expression &condition, const AST &body);
        [[nodiscard]] Expression getCondition() const;
        [[nodiscard]] AST getBody() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_WHILE_H
