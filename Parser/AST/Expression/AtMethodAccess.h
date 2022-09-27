//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_ATMETHODACCESS_H
#define COOLCOMPILER_ATMETHODACCESS_H

#include <vector>
#include "Expression.h"

namespace CoolCompiler {

    class AtMethodAccess : public Expression {
    private:
        Expression* instance;
        std::string type;
        std::string method;
        std::vector<Expression*> arguments;
    public:
        explicit AtMethodAccess(Expression* instance, const std::string &type,
                       const std::string &method, const std::vector<Expression*> &arguments);
        [[nodiscard]] Expression* getInstance() const;
        [[nodiscard]] std::string getType() const;
        [[nodiscard]] std::string getMethod() const;
        [[nodiscard]] std::vector<Expression*> getArguments() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_ATMETHODACCESS_H
