//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_CASEACTION_H
#define COOLCOMPILER_CASEACTION_H

#include "Expression.h"

namespace CoolCompiler {

    class CaseAction : Expression {
    private:
        std::string name;
        std::string type;
        Expression* expression;
    public:
        CaseAction(const std::string &name, const std::string &type, Expression *expression);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::string getType() const;
        [[nodiscard]] Expression* getExpression() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_CASEACTION_H
