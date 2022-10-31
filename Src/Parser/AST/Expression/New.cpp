//
// Created by luoli on 2022/9/18.
//

#include "New.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    New::New(const std::string &type) : Expression("new") {
        this->type = type;
    }

    std::string New::getType() const {
        return type;
    }

    std::string New::typeCheck(SemanticAnalyzer *analyzer) {
        if(type != "SELF_TYPE" && !analyzer->isTypeDefined(type)){
            std::string message = fmt::format("{}: Tried to instantiate an object of undefined type: {}.",
                                              "New", type);
            analyzer->fail(message);

            return "Object";
        }

        return type;
    }
} // CoolCompiler