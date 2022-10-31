//
// Created by luoli on 2022/9/18.
//

#include "Block.h"
#include "../../../Semantic/SemanticAnalyzer.h"

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
} // CoolCompiler