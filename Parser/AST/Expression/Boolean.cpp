//
// Created by luoli on 2022/9/18.
//

#include "Boolean.h"

namespace CoolCompiler {
    Boolean::Boolean(bool value) : Expression("boolean") {
        this->value = value;
    }

    bool Boolean::getValue() const {
        return value;
    }
} // CoolCompiler