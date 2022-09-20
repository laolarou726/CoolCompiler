//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_NOT_H
#define COOLCOMPILER_NOT_H

#include "Expression.h"

namespace CoolCompiler {

    class Not : Expression {
    private:
        Expression expression;
    public:
        explicit Not(const Expression &expression);
        [[nodiscard]] Expression getExpression() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_NOT_H
