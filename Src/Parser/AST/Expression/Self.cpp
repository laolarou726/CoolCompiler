//
// Created by luoli on 2022/9/18.
//

#include "Self.h"
#include "../../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    Self::Self() : Expression("self") {

    }

    std::string Self::typeCheck(SemanticAnalyzer *analyzer) {
        return "SELF_TYPE";
    }
} // CoolCompiler