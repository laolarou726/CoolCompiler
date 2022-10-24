//
// Created by luoli on 2022/10/20.
//

#include <set>
#include "SemanticAnalyzer.h"
#include "../Parser/AST/FeatureMethod.h"
#include "../Parser/AST/FeatureAttribute.h"
#include "fmt/format.h"

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
        for(AST* ast : *program->getClasses()){
            if(ast->getIdentifier() != "class") continue;

            auto* class_ = (Class*) ast;
            std::string className = class_->getName();

            if(className == OBJECT_CLASS->getName() ||
                className == INT_CLASS->getName() ||
                className == BOOL_CLASS->getName() ||
                className == STRING_CLASS->getName() ||
                className == "SELF_TYPE"){
                std::string message = fmt::format("Redefinition of {} is not allowed.", className);
                fail(message);

                return false;
            }

            if(isTypeDefined(className)){
                std::string message = fmt::format("Class {} is already", className);
                fail(message);

                return false;
            }

            classLookups[className] = class_;

            return true;
        }
    }

    bool SemanticAnalyzer::buildInheritanceGraph() {
        for(auto [className, class_] : classLookups){
            if(className == OBJECT_CLASS->getName()) continue;

            std::string inheritFrom = class_->getInherits();

            inheritanceParents[className] = inheritFrom;

            if(inheritFrom == STRING_CLASS->getName() ||
                inheritFrom == INT_CLASS->getName() ||
                inheritFrom == BOOL_CLASS->getName() ||
                inheritFrom == "SELF_TYPE"){
                std::string message = fmt::format("Class {} cannot inherit class {}.", className, inheritFrom);
                fail(message);

                return false;
            }

            if(isTypeDefined(inheritFrom)){
                std::string message = fmt::format("Class {} inherits from an undefined class {}.", className, inheritFrom);
                fail(message);

                return false;
            }

            if(inheritances.find(inheritFrom) == inheritances.end())
                inheritances[inheritFrom] = std::vector<std::string>();

            inheritances[inheritFrom].emplace_back(className);
        }

        return true;
    }

    bool SemanticAnalyzer::graphDFS(std::unordered_map<std::string, int> &visitHistory, const std::string &type) {
        visitHistory[type] = 0;

        for(auto [className, _] : inheritances){
            if(visitHistory[className] == 0){
                std::string message =
                        fmt::format("There exists an (in)direct circular dependency between: {} and {}",
                                    type, className);
                fail(message);

                return false;
            }
            else{
                if(!graphDFS(visitHistory, className)) return false;
            }
        }

        visitHistory[type] = 1;
        return true;
    }

    bool SemanticAnalyzer::isGraphAcyclic() {
        std::unordered_map<std::string, int> visitHistory;

        for(auto [className, _] : classLookups)
            visitHistory[className] = 2;

        for(auto [className, _] : classLookups){
            if(visitHistory[className] == 2)
                if(!graphDFS(visitHistory, className))
                    return false;
        }

        return true;
    }

    bool SemanticAnalyzer::isValid() {
        if(!isGraphAcyclic()) return false;
        if(!isTypeDefined("Main")){
            fail("Class Main is not defined.");
            return false;
        }

        return true;
    }

    bool SemanticAnalyzer::isSubtype(const std::string &candidate, const std::string &target) {
        if(candidate == "_no_type") return true;

        bool isCurrentClass = false;

        if(candidate == "SELF_TYPE"){
            if(target == "SELF_TYPE")
                return true;

            isCurrentClass = true;
        }

        std::string current = isCurrentClass ? currentClassName : candidate;

        while(current != OBJECT_CLASS->getName() && current != target)
            current = inheritanceParents[current];

        return current == target;
    }

    bool SemanticAnalyzer::isTypeDefined(const std::string &type) {
        return classLookups.find(type) != classLookups.end();
    }

    bool SemanticAnalyzer::isPrimitive(const std::string &type) {
        return type == OBJECT_CLASS->getName() ||
                type == IO_CLASS->getName() ||
                type == INT_CLASS->getName() ||
                type == BOOL_CLASS->getName() ||
                type == STRING_CLASS->getName();
    }

    std::string SemanticAnalyzer::leastCommonAncestorType(const std::string &lhs, const std::string &rhs) {
        bool isLeftSelfType = lhs == "SELF_TYPE";
        bool isRightSelfType = rhs == "SELF_TYPE";

        std::string currentLhs = isLeftSelfType ? currentClassName : lhs;
        std::string currentRhs = isRightSelfType ? currentClassName : lhs;
        std::set<std::string> rhsAncestors;

        while(currentRhs != OBJECT_CLASS->getName()){
            rhsAncestors.emplace(currentRhs);
            currentRhs = inheritanceParents[currentRhs];
        }

        while(currentLhs != OBJECT_CLASS->getName()){
            if(rhsAncestors.find(currentLhs) != rhsAncestors.end())
                return currentLhs;

            currentLhs = inheritanceParents[currentLhs];
        }

        return OBJECT_CLASS->getName();
    }

    std::string SemanticAnalyzer::getParentType(const std::string &type) {
        if(inheritanceParents.find(type) == inheritanceParents.end())
            return "_no_type";

        return inheritanceParents[type];
    }

    void SemanticAnalyzer::doCheck() {

    }

    void SemanticAnalyzer::fail(const std::string &message) {
        errorCount++;
        std::cerr << message << std::endl;
    }

    int SemanticAnalyzer::getErrorCount() const {
        return errorCount;
    }

} // CoolCompiler