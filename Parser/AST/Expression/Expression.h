//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_EXPRESSION_H
#define COOLCOMPILER_EXPRESSION_H

#include "../AST.h"

namespace CoolCompiler {

    class Expression : public AST {
    protected:
        explicit Expression(const std::string &identifier);
    public:
        Expression();
    };

} // CoolCompiler

#endif //COOLCOMPILER_EXPRESSION_H
