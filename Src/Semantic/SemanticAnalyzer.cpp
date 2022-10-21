//
// Created by luoli on 2022/10/20.
//

#include "SemanticAnalyzer.h"
#include "../Parser/AST/FeatureMethod.h"
#include "../Parser/AST/FeatureAttribute.h"

namespace CoolCompiler {

    SemanticAnalyzer::SemanticAnalyzer() {
        //OBJECT CLASS
        std::vector<AST*> objectFeatures;
        objectFeatures.emplace_back(new FeatureMethod("abort", "Object", nullptr));
        objectFeatures.emplace_back(new FeatureMethod("type_name", "String", nullptr));
        objectFeatures.emplace_back(new FeatureMethod("copy", "SELF_TYPE", nullptr));

        OBJECT_CLASS = new Class("Object", objectFeatures, "");

        //IO CLASS
        std::vector<AST*> ioFeatures;

        std::vector<Formal*> outStringArguments;
        outStringArguments.emplace_back(new Formal("arg", "String"));
        ioFeatures.emplace_back(new FeatureMethod("out_string", "SELF_TYPE", nullptr, outStringArguments));

        std::vector<Formal*> outIntArguments;
        outIntArguments.emplace_back(new Formal("arg", "Int"));
        ioFeatures.emplace_back(new FeatureMethod("out_int", "SELF_TYPE", nullptr, outIntArguments));

        ioFeatures.emplace_back(new FeatureMethod("in_string", "String", nullptr));
        ioFeatures.emplace_back(new FeatureMethod("in_int", "Int", nullptr));

        IO_CLASS = new Class("IO", ioFeatures, OBJECT_CLASS->getName());

        //INT CLASS
        std::vector<AST*> intFeatures;
        intFeatures.emplace_back(new FeatureAttribute("val", "Int"));

        INT_CLASS = new Class("Int", intFeatures, OBJECT_CLASS->getName());

        //BOOL CLASS
        std::vector<AST*> boolFeatures;
        boolFeatures.emplace_back(new FeatureAttribute("val", "_prim_slot"));

        BOOL_CLASS = new Class("Bool", boolFeatures, OBJECT_CLASS->getName());

        //STRING CLASS
        std::vector<AST*> stringFeatures;
        stringFeatures.emplace_back(new FeatureAttribute("val", INT_CLASS->getName()));
        stringFeatures.emplace_back(new FeatureAttribute("str_field", "_prim_slot"));
        stringFeatures.emplace_back(new FeatureMethod("length", INT_CLASS->getName(), nullptr));

        std::vector<Formal*> concatArguments;
        concatArguments.emplace_back(new Formal("arg", STRING_CLASS->getName()));
        stringFeatures.emplace_back(new FeatureMethod("concat", STRING_CLASS->getName(), nullptr, concatArguments));

        std::vector<Formal*> substrArguments;
        substrArguments.emplace_back(new Formal("arg", INT_CLASS->getName()));
        substrArguments.emplace_back(new Formal("arg2", INT_CLASS->getName()));
        stringFeatures.emplace_back(new FeatureMethod("substr", STRING_CLASS->getName(), nullptr, substrArguments));

        STRING_CLASS = new Class("String", stringFeatures, OBJECT_CLASS->getName());

        //ADD
        classLookups[OBJECT_CLASS->getName()] = OBJECT_CLASS;
        classLookups[IO_CLASS->getName()] = IO_CLASS;
        classLookups[INT_CLASS->getName()] = INT_CLASS;
        classLookups[BOOL_CLASS->getName()] = BOOL_CLASS;
        classLookups[STRING_CLASS->getName()] = STRING_CLASS;
    }

    SemanticAnalyzer::SemanticAnalyzer(Program* program) : SemanticAnalyzer() {
        this->program = program;
    }

    bool SemanticAnalyzer::resolveDefinedClasses() {
        return false;
    }

    bool SemanticAnalyzer::buildInheritanceGraph() {
        return false;
    }

    bool SemanticAnalyzer::isGraphAcyclic() {
        return false;
    }

    bool SemanticAnalyzer::isValid() {
        return false;
    }

    bool SemanticAnalyzer::isSubtype(const std::string &type1, const std::string &type2) {
        return false;
    }

    bool SemanticAnalyzer::isTypeDefined(const std::string &type) {
        return false;
    }

    bool SemanticAnalyzer::isPrimitive(const std::string &type) {
        return false;
    }

    void SemanticAnalyzer::doCheck() {

    }

    std::vector<Error> SemanticAnalyzer::getErrors() const {
        return errors;
    }
} // CoolCompiler