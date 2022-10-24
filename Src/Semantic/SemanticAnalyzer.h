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
        int errorCount;
        std::string currentClassName;
        Program* program = nullptr;
        std::unordered_map<std::string, Class*> classLookups;
        std::unordered_map<std::string, std::vector<std::string>> inheritances;
        std::unordered_map<std::string, std::string> inheritanceParents;

        Class* OBJECT_CLASS;
        Class* IO_CLASS;
        Class* INT_CLASS;
        Class* BOOL_CLASS;
        Class* STRING_CLASS;

        SemanticAnalyzer();

        void fail(const std::string &message);

        bool resolveDefinedClasses();
        bool buildInheritanceGraph();
        bool graphDFS(std::unordered_map<std::string, int> &visitHistory, const std::string &type);
        bool isGraphAcyclic();
        bool isValid();
        bool isSubtype(const std::string &candidate, const std::string &target);
        bool isTypeDefined(const std::string &type);
        bool isPrimitive(const std::string &type);

        std::string leastCommonAncestorType(const std::string &lhs, const std::string &rhs);
        std::string getParentType(const std::string &type);
    public:
        explicit SemanticAnalyzer(Program* program);
        void doCheck();
        int getErrorCount() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_SEMANTICANALYZER_H
