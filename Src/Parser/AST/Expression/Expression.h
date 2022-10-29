//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_EXPRESSION_H
#define COOLCOMPILER_EXPRESSION_H

#include "../AST.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {

    class Expression : public AST {
    protected:
        explicit Expression(const std::string &identifier);
    public:
        Expression();
        void print(int depth) override = 0;
        virtual std::string typeCheck(SemanticAnalyzer* analyzer) = 0;
    };

} // CoolCompiler

#endif //COOLCOMPILER_EXPRESSION_H
