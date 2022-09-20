//
// Created by luoli on 2022/9/18.
//

#include "New.h"

namespace CoolCompiler {
    New::New(const std::string &type) : Expression("new") {
        this->type = type;
    }

    std::string New::getType() const {
        return type;
    }
} // CoolCompiler