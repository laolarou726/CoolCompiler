//
// Created by luoli on 2022/9/18.
//

#include "Id.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    Id::Id(const std::string &name) : Expression("id") {
        this->name = name;
    }

    Id::Id() : Expression("id") {}

    std::string Id::getName() const {
        return name;
    }

    std::string Id::typeCheck(SemanticAnalyzer *analyzer) {
        auto* objectsTable = analyzer->getObjectsTable();
        std::string* result = objectsTable->lookup(name);

        if(result == nullptr){
            std::string message = fmt::format("Tried to access undeclared identifier [{}].",
                                              name);
            analyzer->fail(message);

            return "Object";
        }

        return *result;
    }
} // CoolCompiler