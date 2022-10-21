//
// Created by luoli on 2022/10/20.
//

#ifndef COOLCOMPILER_SEMANTICANALYZER_H
#define COOLCOMPILER_SEMANTICANALYZER_H

#include "../Parser/AST/Class.h"
#include "../Parser/AST/Program.h"
#include "unordered_map"

namespace CoolCompiler {

    class SemanticAnalyzer {
    private:
        Program* program;
    public:
        explicit SemanticAnalyzer(Program* program);
        void doCheck();
    };

} // CoolCompiler

#endif //COOLCOMPILER_SEMANTICANALYZER_H
