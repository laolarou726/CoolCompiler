//
// Created by luoli on 2022/10/20.
//

#ifndef COOLCOMPILER_CLASSTABLE_H
#define COOLCOMPILER_CLASSTABLE_H

#include "../Parser/AST/Program.h"
#include "../Parser/AST/Class.h"
#include "unordered_map"

namespace CoolCompiler {

    class ClassTable {
    private:
        Program* program = nullptr;
        std::unordered_map<std::string, Class*> classLookups;
        Class* OBJECT_CLASS;
        Class* IO_CLASS;
        Class* INT_CLASS;
        Class* BOOL_CLASS;
        Class* STRING_CLASS;
        ClassTable();
    public:
        explicit ClassTable(Program* program);
        bool resolveDefinedClasses();
        bool buildInheritanceGraph();
        bool isGraphAcyclic();
        bool isValid();
        bool isSubtype(const std::string &type1, const std::string &type2);
        bool isTypeDefined(const std::string &type);
        bool isPrimitive(const std::string &type);
    };

} // CoolCompiler

#endif //COOLCOMPILER_CLASSTABLE_H
