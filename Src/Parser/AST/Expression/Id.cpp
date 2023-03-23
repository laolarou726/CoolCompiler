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
        if(name == "self")
            return "SELF_TYPE";

        auto* objectsTable = analyzer->getObjectsTable();
        std::string* result = objectsTable->lookup(name);

        if(result == nullptr){
            std::string message = fmt::format("The referenced object [{}] is undefined in relevant scopes.",
                                              name);
            analyzer->fail(message);

            return "Object";
        }

        return *result;
    }

    llvm::Value *Id::visit(CoolCompiler::CodeGenerator *generator) {
        return nullptr;
    }
} // CoolCompiler