//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_METHODACCESS_H
#define COOLCOMPILER_METHODACCESS_H

#include <vector>
#include "Expression.h"

namespace CoolCompiler {

    class MethodAccess : public Expression {
    private:
        Expression instance;
        std::string method;
        std::vector<Expression> arguments;
    public:
        explicit MethodAccess(const Expression &instance, const std::string &method, const std::vector<Expression> &arguments);
        [[nodiscard]] Expression getInstance() const;
        [[nodiscard]] std::string getMethod() const;
        [[nodiscard]] std::vector<Expression> getArguments() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_METHODACCESS_H
