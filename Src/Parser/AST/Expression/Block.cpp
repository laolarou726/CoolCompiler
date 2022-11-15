//
// Created by luoli on 2022/9/18.
//

#include "Block.h"
#include "../../../Semantic/SemanticAnalyzer.h"
#include "../../../CodeGen/CodeGenerator.h"

namespace CoolCompiler {
    Block::Block(const std::vector<Expression*> &expressions) : Expression("block") {
        this->expressions = expressions;
    }

    std::vector<Expression*> Block::getExpressions() const {
        return expressions;
    }

    std::string Block::typeCheck(SemanticAnalyzer *analyzer) {
        std::string result = "Object";

        for(auto* expr : expressions)
            result = expr->typeCheck(analyzer);

        return result;
    }

    llvm::Value *Block::visit(CoolCompiler::CodeGenerator *generator) {
        llvm::Value* lastExprValue = nullptr;
        for(int i = 0; i < expressions.size(); i++){
            Expression* expr = expressions[i];

            if(i == expressions.size() - 1)
                lastExprValue = expr->visit(generator);
            else
                expr->visit(generator);
        }

        return lastExprValue;
    }
} // CoolCompiler