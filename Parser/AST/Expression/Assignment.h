//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_ASSIGNMENT_H
#define COOLCOMPILER_ASSIGNMENT_H

#include "Expression.h"
#include "Id.h"

namespace CoolCompiler {

    class Assignment : Expression {
    private:
        Id id;
        Expression expression;
    public:
        Assignment(const Id &id, const Expression &expression);
        [[nodiscard]] Id getId() const;
        [[nodiscard]] Expression getExpression() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_ASSIGNMENT_H
