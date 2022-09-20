//
// Created by luoli on 2022/9/18.
//

#include "String.h"

namespace CoolCompiler {
    String::String(const std::string &value) : Expression("string") {
        this->value = value;
    }

    std::string String::getValue() const {
        return value;
    }
} // CoolCompiler