//
// Created by luoli on 2022/9/18.
//

#include "SelfMethodAccess.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    SelfMethodAccess::SelfMethodAccess(const std::string &method, const std::vector<Expression*> &arguments) : Expression("self_method_access") {
        this->method = method;
        this->arguments = arguments;
    }

    std::string SelfMethodAccess::getMethod() const {
        return method;
    }

    std::vector<Expression*> SelfMethodAccess::getArguments() const {
        return arguments;
    }

    std::string SelfMethodAccess::typeCheck(SemanticAnalyzer *analyzer) {
        return "";
    }
} // CoolCompiler