//
// Created by luoli on 2022/9/18.
//

#include "Integer.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    Integer::Integer(int value) : Expression("integer") {
        this->value = value;
    }

    int Integer::getValue() const {
        return value;
    }

    std::string Integer::typeCheck(SemanticAnalyzer *analyzer) {
        return "Int";
    }
} // CoolCompiler