//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_FORMAL_H
#define COOLCOMPILER_FORMAL_H

#include "AST.h"

namespace CoolCompiler {

    class Formal : AST {
    private:
        std::string name;
        std::string type;
    public:
        Formal(const std::string &name, const std::string &type);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::string getType() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_FORMAL_H
