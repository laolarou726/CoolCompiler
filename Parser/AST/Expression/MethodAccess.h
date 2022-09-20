//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_METHODACCESS_H
#define COOLCOMPILER_METHODACCESS_H

#include <vector>
#include "Expression.h"

namespace CoolCompiler {

    class MethodAccess : Expression {
    private:
        AST instance;
        std::string method;
        std::vector<AST> arguments;
    public:
        explicit MethodAccess(const AST &instance, const std::string &method, const std::vector<AST> &arguments);
        [[nodiscard]] AST getInstance() const;
        [[nodiscard]] std::string getMethod() const;
        [[nodiscard]] std::vector<AST> getArguments() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_METHODACCESS_H
