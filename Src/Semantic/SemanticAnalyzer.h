//
// Created by luoli on 2022/10/20.
//

#ifndef COOLCOMPILER_SEMANTICANALYZER_H
#define COOLCOMPILER_SEMANTICANALYZER_H

#include "../Parser/AST/Class.h"
#include "../Parser/AST/Program.h"
#include "map"
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
        std::map<std::string, Class*> classLookups;
        std::map<std::string, std::vector<std::string>> inheritances;
        std::map<std::string, std::string> inheritanceParents;
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
        bool graphDFS(std::map<std::string, int> &visitHistory, const std::string &type);
        bool isGraphAcyclic();
        bool isValid();
    public:
        explicit SemanticAnalyzer(Program* program);
        void doCheck();
        int getErrorCount() const;
        bool isTypeDefined(const std::string &type);
        void fail(const std::string &message, bool exit = false);
        bool isPrimitive(const std::string &type);
        SymbolTable<std::string, std::string>* getObjectsTable() const;

        std::string leastCommonAncestorType(const std::string &lhs, const std::string &rhs);
        std::string getCurrentClassName() const;

        FeatureMethod* lookupMethodInChain(const std::string &className, const std::string &method);
        FeatureMethod* lookupMethod(const std::string &className, const std::string &method);

        bool isSubtype(const std::string &candidate, const std::string &target);

        void ensureAttributesUnique(Class* class_);

        void buildAttributeScopes(const Class* class_);

        void processMethod(Class* class_, FeatureMethod* method, FeatureMethod* parentMethod);

        std::string getParentType(const std::string &type);

        Class* getParentClass(const std::string &type);

        void setCurrentClassName(const std::string &name);

        void processAttribute(Class* class_, FeatureAttribute* attr);
    };

} // CoolCompiler

#endif //COOLCOMPILER_SEMANTICANALYZER_H
