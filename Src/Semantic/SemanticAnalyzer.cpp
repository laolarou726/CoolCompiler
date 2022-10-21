//
// Created by luoli on 2022/10/20.
//

#include "SemanticAnalyzer.h"
#include "../Parser/AST/FeatureMethod.h"
#include "../Parser/AST/FeatureAttribute.h"

namespace CoolCompiler {
    SemanticAnalyzer::SemanticAnalyzer(Program* program) {
        this->program = program;
    }

    void SemanticAnalyzer::doCheck() {

    }
} // CoolCompiler