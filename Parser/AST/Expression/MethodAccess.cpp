//
// Created by luoli on 2022/9/18.
//

#include "MethodAccess.h"

namespace CoolCompiler {
    MethodAccess::MethodAccess(Expression* instance,
                               const std::string &method,
                               const std::vector<Expression*> &arguments) : Expression("method_access") {
        this->instance = instance;
        this->method = method;
        this->arguments = arguments;
    }

    Expression* MethodAccess::getInstance() const {
        return instance;
    }

    std::string MethodAccess::getMethod() const {
        return method;
    }

    std::vector<Expression*> MethodAccess::getArguments() const {
        return arguments;
    }
} // CoolCompiler