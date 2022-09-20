//
// Created by luoli on 2022/9/18.
//

#include "Class.h"

namespace CoolCompiler {
    Class::Class(const std::string &name, const std::vector<AST> &features, const std::string &inherits) : AST("class") {
        this->name = name;
        this->features = features;
        this->inherits = inherits;
    }

    std::string Class::getName() const {
        return name;
    }

    std::vector<AST> Class::getFeatures() const {
        return features;
    }

    std::string Class::getInherits() const {
        return inherits;
    }
} // CoolCompiler