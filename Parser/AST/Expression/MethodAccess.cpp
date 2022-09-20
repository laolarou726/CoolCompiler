//
// Created by luoli on 2022/9/18.
//

#include "MethodAccess.h"

namespace CoolCompiler {
    MethodAccess::MethodAccess(const AST &instance,
                               const std::string &method,
                               const std::vector<AST> &arguments) : Expression("method_access") {
        this->instance = instance;
        this->method = method;
        this->arguments = arguments;
    }

    AST MethodAccess::getInstance() const {
        return instance;
    }

    std::string MethodAccess::getMethod() const {
        return method;
    }

    std::vector<AST> MethodAccess::getArguments() const {
        return arguments;
    }
} // CoolCompiler