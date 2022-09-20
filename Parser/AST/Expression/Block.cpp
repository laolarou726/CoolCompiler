//
// Created by luoli on 2022/9/18.
//

#include "Block.h"

namespace CoolCompiler {
    Block::Block(const std::vector<Expression> &expressions) : Expression("block") {
        this->expressions = expressions;
    }

    std::vector<Expression> Block::getExpressions() const {
        return expressions;
    }
} // CoolCompiler