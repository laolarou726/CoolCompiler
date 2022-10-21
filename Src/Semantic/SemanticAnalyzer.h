//
// Created by luoli on 2022/10/20.
//

#ifndef COOLCOMPILER_SEMANTICANALYZER_H
#define COOLCOMPILER_SEMANTICANALYZER_H

#include "../Parser/AST/Class.h"
#include "../Parser/AST/Program.h"
#include "unordered_map"
#include "../Lexer/Error.h"

namespace CoolCompiler {

    class SemanticAnalyzer {
    private:
        std::vector<Error> errors;

        Program* program = nullptr;
        std::unordered_map<std::string, Class*> classLookups;

        Class* OBJECT_CLASS;
        Class* IO_CLASS;
        Class* INT_CLASS;
        Class* BOOL_CLASS;
        Class* STRING_CLASS;

        SemanticAnalyzer();

        bool resolveDefinedClasses();
        bool buildInheritanceGraph();
        bool isGraphAcyclic();
        bool isValid();
        bool isSubtype(const std::string &type1, const std::string &type2);
        bool isTypeDefined(const std::string &type);
        bool isPrimitive(const std::string &type);
    public:
        explicit SemanticAnalyzer(Program* program);
        void doCheck();
        std::vector<Error> getErrors() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_SEMANTICANALYZER_H
