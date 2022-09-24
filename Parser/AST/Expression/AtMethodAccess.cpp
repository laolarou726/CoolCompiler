//
// Created by luoli on 2022/9/18.
//

#include "AtMethodAccess.h"

namespace CoolCompiler {
    AtMethodAccess::AtMethodAccess(const Expression &instance, const std::string &type, const std::string &method,
                                   const std::vector<Expression> &arguments) : Expression("at_method_access") {
        this->instance = instance;
        this->type = type;
        this->method = method;
        this->arguments = arguments;
    }

    Expression AtMethodAccess::getInstance() const {
        return instance;
    }

    std::string AtMethodAccess::getType() const {
        return type;
    }

    std::string AtMethodAccess::getMethod() const {
        return method;
    }

    std::vector<Expression> AtMethodAccess::getArguments() const {
        return arguments;
    }
} // CoolCompiler