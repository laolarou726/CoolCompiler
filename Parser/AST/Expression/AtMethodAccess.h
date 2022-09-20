//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_ATMETHODACCESS_H
#define COOLCOMPILER_ATMETHODACCESS_H

#include <vector>
#include "Expression.h"

namespace CoolCompiler {

    class AtMethodAccess : Expression {
    private:
        AST instance;
        std::string type;
        std::string method;
        std::vector<AST> arguments;
    public:
        explicit AtMethodAccess(const AST &instance, const std::string &type,
                       const std::string &method, const std::vector<AST> &arguments);
        [[nodiscard]] AST getInstance() const;
        [[nodiscard]] std::string getType() const;
        [[nodiscard]] std::string getMethod() const;
        [[nodiscard]] std::vector<AST> getArguments() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_ATMETHODACCESS_H
