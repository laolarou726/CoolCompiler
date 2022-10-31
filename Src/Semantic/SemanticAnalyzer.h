//
// Created by luoli on 2022/10/20.
//

#ifndef COOLCOMPILER_SEMANTICANALYZER_H
#define COOLCOMPILER_SEMANTICANALYZER_H

#include "../Parser/AST/Class.h"
#include "../Parser/AST/Program.h"
#include "unordered_map"
#include "../Lexer/Error.h"
#include "SymbolTable/SymbolTable.h"
#include "../Parser/AST/Feature/FeatureAttribute.h"
#include "../Parser/AST/Feature/FeatureMethod.h"

namespace CoolCompiler {

    class SemanticAnalyzer {
    private:
        int errorCount;
        std::string currentClassName;
        Program* program = nullptr;
        std::unordered_map<std::string, Class*> classLookups;
        std::unordered_map<std::string, std::vector<std::string>> inheritances;
        std::unordered_map<std::string, std::string> inheritanceParents;
        SymbolTable<std::string, std::string>* objectsTable;

        Class* OBJECT_CLASS;
        Class* IO_CLASS;
        Class* INT_CLASS;
        Class* BOOL_CLASS;
        Class* STRING_CLASS;

        SemanticAnalyzer();

        //CLASS CHECKS
        bool resolveDefinedClasses();
        bool buildInheritanceGraph();
        bool graphDFS(std::unordered_map<std::string, int> &visitHistory, const std::string &type);
        bool isGraphAcyclic();
        bool isValid();

        void ensureAttributesUnique(Class* class_);

        std::string getParentType(const std::string &type);

        void buildAttributeScopes(const Class* class_);
        void processAttribute(Class* class_, FeatureAttribute* attr);
        void processMethod(Class* class_, FeatureMethod* method, FeatureMethod* parentMethod);
        void typeCheck(Class* class_);
    public:
        explicit SemanticAnalyzer(Program* program);
        void doCheck();
        int getErrorCount() const;
        bool isTypeDefined(const std::string &type);
        void fail(const std::string &message);
        bool isPrimitive(const std::string &type);
        SymbolTable<std::string, std::string>* getObjectsTable() const;

        std::string leastCommonAncestorType(const std::string &lhs, const std::string &rhs);
        std::string getCurrentClassName() const;

        FeatureMethod* lookupMethodInChain(const std::string &className, const std::string &method);
        FeatureMethod* lookupMethod(const std::string &className, const std::string &method);

        bool isSubtype(const std::string &candidate, const std::string &target);
    };

} // CoolCompiler

#endif //COOLCOMPILER_SEMANTICANALYZER_H
