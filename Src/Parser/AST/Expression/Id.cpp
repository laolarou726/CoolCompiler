//
// Created by luoli on 2022/9/18.
//

#include "Id.h"

namespace CoolCompiler {
    Id::Id(const std::string &name) : Expression("id") {
        this->name = name;
    }

    Id::Id() : Expression("id") {}

    std::string Id::getName() const {
        return name;
    }
} // CoolCompiler