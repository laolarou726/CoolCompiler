//
// Created by luoli on 2022/9/18.
//

#include "AtMethodAccess.h"

namespace CoolCompiler {
    AtMethodAccess::AtMethodAccess(const AST &instance, const std::string &type, const std::string &method,
                                   const std::vector<AST> &arguments) : Expression("at_method_access") {
        this->instance = instance;
        this->type = type;
        this->method = method;
        this->arguments = arguments;
    }

    AST AtMethodAccess::getInstance() const {
        return instance;
    }

    std::string AtMethodAccess::getType() const {
        return type;
    }

    std::string AtMethodAccess::getMethod() const {
        return method;
    }

    std::vector<AST> AtMethodAccess::getArguments() const {
        return arguments;
    }
} // CoolCompiler