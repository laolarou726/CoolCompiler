//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_IF_H
#define COOLCOMPILER_IF_H

#include "Expression.h"

namespace CoolCompiler {

    class If : Expression {
    private:
        Expression condition;
        AST conditionTrue;
        AST conditionFalse;
    public:
        If(const Expression &condition, const AST &conditionTrue, const AST& conditionFalse);
        [[nodiscard]] Expression getCondition() const;
        [[nodiscard]] AST getConditionTrue() const;
        [[nodiscard]] AST getConditionFalse() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_IF_H
