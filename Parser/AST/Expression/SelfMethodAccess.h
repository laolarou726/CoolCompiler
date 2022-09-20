//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_SELFMETHODACCESS_H
#define COOLCOMPILER_SELFMETHODACCESS_H

#include <vector>
#include "Expression.h"

namespace CoolCompiler {

    class SelfMethodAccess : Expression {
    private:
        std::string method;
        std::vector<AST> arguments;
    public:
        explicit SelfMethodAccess(const std::string &method, const std::vector<AST> &arguments);
        [[nodiscard]] std::string getMethod() const;
        [[nodiscard]] std::vector<AST> getArguments() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_SELFMETHODACCESS_H
