//
// Created by luoli on 2022/9/18.
//

#include "SelfMethodAccess.h"

namespace CoolCompiler {
    SelfMethodAccess::SelfMethodAccess(const std::string &method, const std::vector<AST> &arguments) : Expression("self_method_access") {
        this->method = method;
        this->arguments = arguments;
    }

    std::string SelfMethodAccess::getMethod() const {
        return method;
    }

    std::vector<AST> SelfMethodAccess::getArguments() const {
        return arguments;
    }
} // CoolCompiler