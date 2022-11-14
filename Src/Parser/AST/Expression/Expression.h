//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_EXPRESSION_H
#define COOLCOMPILER_EXPRESSION_H

#include "../AST.h"
#include <llvm-15/llvm/IR/Value.h>

namespace CoolCompiler {

    class SemanticAnalyzer;
    class CodeGenerator;

    class Expression : public AST {
    protected:
        explicit Expression(const std::string &identifier);
    public:
        Expression();
        void print(int depth) override = 0;
        virtual std::string typeCheck(SemanticAnalyzer* analyzer) = 0;
        virtual llvm::Value* visit(CodeGenerator* generator) = 0;
    };

    class PlaceholderExpr : public Expression{
    public:
        explicit PlaceholderExpr(const std::string &id) : Expression(id){}
        void print(int depth) override{}
        std::string typeCheck(CoolCompiler::SemanticAnalyzer *analyzer) override {return "";}
        llvm::Value* visit(CoolCompiler::CodeGenerator *generator) override {return nullptr;}
    };

} // CoolCompiler

#endif //COOLCOMPILER_EXPRESSION_H
