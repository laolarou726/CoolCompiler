//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_PROGRAM_H
#define COOLCOMPILER_PROGRAM_H

#include <vector>
#include "AST.h"

namespace CoolCompiler {

    class Program : AST {
    private:
        std::vector<AST> classes;
    public:
        explicit Program(const std::vector<AST> &classes);
        [[nodiscard]] std::vector<AST> getClasses() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_PROGRAM_H