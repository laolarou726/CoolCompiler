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

        static Class* OBJECT_CLASS;
        static Class* IO_CLASS;
        static Class* INT_CLASS;
        static Class* BOOL_CLASS;
        static Class* STRING_CLASS;

        //CLASS CHECKS
        bool resolveDefinedClasses();
        bool buildInheritanceGraph();
        bool graphDFS(std::map<std::string, int> &visitHistory, const std::string &type);
        bool isGraphAcyclic();
        bool isValid();
    public:
        SemanticAnalyzer();
        explicit SemanticAnalyzer(Program* program);

        static void initBaseTypes();

        void doCheck();
        [[nodiscard]] int getErrorCount() const;
        bool isTypeDefined(const std::string &type);
        void fail(const std::string &message, bool exit = false);
        static bool isPrimitive(const std::string &type);
        [[nodiscard]] SymbolTable<std::string, std::string>* getObjectsTable() const;

        std::string leastCommonAncestorType(const std::string &lhs, const std::string &rhs);
        [[nodiscard]] std::string getCurrentClassName() const;

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

        [[nodiscard]] Class* GET_OBJECT_CLASS() const;
        [[nodiscard]] Class* GET_IO_CLASS() const;
        [[nodiscard]] Class* GET_INT_CLASS() const;
        [[nodiscard]] Class* GET_BOOL_CLASS() const;
        [[nodiscard]] Class* GET_STRING_CLASS() const;

        [[nodiscard]] Program* getProgram() const;
        [[nodiscard]] std::vector<Class*> getInheritList(Class* class_);
    };

} // CoolCompiler

#endif //COOLCOMPILER_SEMANTICANALYZER_H
