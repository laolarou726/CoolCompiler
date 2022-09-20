//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_ISVOID_H
#define COOLCOMPILER_ISVOID_H

#include "Expression.h"

namespace CoolCompiler {

    class IsVoid : Expression {
    private:
        Expression expression;
    public:
        explicit IsVoid(const Expression &expression);
        [[nodiscard]] Expression getExpression() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_ISVOID_H
