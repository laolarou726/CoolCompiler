//
// Created by luoli on 2022/9/18.
//

#include "String.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    String::String(const std::string &value) : Expression("string") {
        this->value = value;
    }

    std::string String::getValue() const {
        return value;
    }

    std::string String::typeCheck(SemanticAnalyzer *analyzer) {
        return "String";
    }
} // CoolCompiler