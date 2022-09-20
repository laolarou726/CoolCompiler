//
// Created by luoli on 2022/9/18.
//

#include "Formal.h"

namespace CoolCompiler {
    Formal::Formal(const std::string &name, const std::string &type) : AST("formal") {
        this->name = name;
        this->type = type;
    }

    std::string Formal::getName() const {
        return name;
    }

    std::string Formal::getType() const {
        return type;
    }
} // CoolCompiler