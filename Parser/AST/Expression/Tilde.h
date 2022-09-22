//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_TILDE_H
#define COOLCOMPILER_TILDE_H

#include "Expression.h"

namespace CoolCompiler {

    class Tilde : public Expression {
    private:
        Expression expression;
    public:
        explicit Tilde(const Expression &expression);
        [[nodiscard]] Expression getExpression() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_TILDE_H
