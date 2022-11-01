//
// Created by luoli on 2022/10/20.
//

#include <set>
#include "SemanticAnalyzer.h"
#include "fmt/format.h"

namespace CoolCompiler {

    SemanticAnalyzer::SemanticAnalyzer() {
        //OBJECT CLASS
        std::vector<FeatureBase*> objectFeatures;
        objectFeatures.emplace_back(new FeatureMethod("abort", "Object", nullptr));
        objectFeatures.emplace_back(new FeatureMethod("type_name", "String", nullptr));
        objectFeatures.emplace_back(new FeatureMethod("copy", "SELF_TYPE", nullptr));

        OBJECT_CLASS = new Class("Object", objectFeatures, "");

        //IO CLASS
        std::vector<FeatureBase*> ioFeatures;

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
        std::vector<FeatureBase*> intFeatures;
        intFeatures.emplace_back(new FeatureAttribute("val", "Int"));

        INT_CLASS = new Class("Int", intFeatures, OBJECT_CLASS->getName());

        //BOOL CLASS
        std::vector<FeatureBase*> boolFeatures;
        boolFeatures.emplace_back(new FeatureAttribute("val", "_prim_slot"));

        BOOL_CLASS = new Class("Bool", boolFeatures, OBJECT_CLASS->getName());

        //STRING CLASS
        std::vector<FeatureBase*> stringFeatures;
        stringFeatures.emplace_back(new FeatureAttribute("val", INT_CLASS->getName()));
        stringFeatures.emplace_back(new FeatureAttribute("str_field", "_prim_slot"));
        stringFeatures.emplace_back(new FeatureMethod("length", INT_CLASS->getName(), nullptr));

        std::vector<Formal*> concatArguments;
        concatArguments.emplace_back(new Formal("arg", "String"));
        stringFeatures.emplace_back(new FeatureMethod("concat", "String", nullptr, concatArguments));

        std::vector<Formal*> substrArguments;
        substrArguments.emplace_back(new Formal("arg", INT_CLASS->getName()));
        substrArguments.emplace_back(new Formal("arg2", INT_CLASS->getName()));
        stringFeatures.emplace_back(new FeatureMethod("substr", "String", nullptr, substrArguments));

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

    //CLASS CHECKS

    bool SemanticAnalyzer::resolveDefinedClasses() {
        for(Class* class_ : *program->getClasses()){
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
        }

        return true;
    }

    bool SemanticAnalyzer::buildInheritanceGraph() {
        for(auto [className, class_] : classLookups){
            if(className == OBJECT_CLASS->getName()) continue;

            std::string inheritFrom = class_->getInherits();
            inheritFrom = inheritFrom.empty() ? OBJECT_CLASS->getName() : inheritFrom;

            inheritanceParents[className] = inheritFrom;

            if(inheritFrom == STRING_CLASS->getName() ||
                inheritFrom == INT_CLASS->getName() ||
                inheritFrom == BOOL_CLASS->getName() ||
                inheritFrom == "SELF_TYPE"){
                std::string message = fmt::format("Class {} cannot inherit class {}.", className, inheritFrom);
                fail(message);

                return false;
            }

            if(!isTypeDefined(inheritFrom)){
                std::string message = fmt::format("Class [{}] inherits from an undefined class [{}].", className, inheritFrom);
                fail(message);

                return false;
            }

            if(inheritances.find(inheritFrom) == inheritances.end())
                inheritances[inheritFrom] = std::vector<std::string>();

            inheritances[inheritFrom].emplace_back(className);
        }

        return true;
    }

    bool SemanticAnalyzer::graphDFS(std::map<std::string, int> &visitHistory, const std::string &type) {
        visitHistory[type] = 0;

        for(std::string className : inheritances[type]){
            if(visitHistory[className] == 0){
                std::string message =
                        fmt::format("There exists an (in)direct circular dependency between: [{}] and [{}]",
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
        std::map<std::string, int> visitHistory;

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

        std::string realTarget = target;
        std::string current = isCurrentClass ? currentClassName : candidate;
        if(isCurrentClass){
            current = realTarget;
            realTarget = currentClassName;
        }

        while(current != OBJECT_CLASS->getName() && current != realTarget)
            current = inheritanceParents[current];

        return current == realTarget;
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

    void SemanticAnalyzer::ensureAttributesUnique(Class *class_) {
        std::set<std::string> attrNamesMap;
        std::string name = class_->getName();

        for(auto* feature : class_->getFeatures()){
            if(dynamic_cast<FeatureMethod*>(feature) != nullptr) continue;

            auto* featureAttr = (FeatureAttribute*) feature;
            std::string attrName = featureAttr->getName();

            if(attrNamesMap.find(attrName) != attrNamesMap.end()){
                std::string message = fmt::format("{}: The attribute [{}] has already been defined!",
                                                  name, attrName);
                fail(message);
                continue;
            }

            attrNamesMap.emplace(attrName);
        }
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
        errorCount = 0;
        objectsTable = new SymbolTable<std::string, std::string>();

        if(!resolveDefinedClasses())
            fail("Compilation halted due to static semantic errors.");
        if(!buildInheritanceGraph())
            fail("Compilation halted due to static semantic errors.");
        if(!isValid())
            fail("Compilation halted due to static semantic errors.");
        if(errorCount != 0)
            fail("Compilation halted due to static semantic errors.");

        for(auto* class_ : *program->getClasses())
            class_->typeCheck(this);

        if(errorCount != 0)
            fail("Compilation halted due to static semantic errors.");
    }

    void SemanticAnalyzer::fail(const std::string &message, bool exit) {
        errorCount++;
        std::cerr << currentClassName << ": " << message << std::endl;

        if(exit)
            std::exit(-1);
    }

    int SemanticAnalyzer::getErrorCount() const {
        return errorCount;
    }

    void SemanticAnalyzer::buildAttributeScopes(const Class *class_) {
        objectsTable->enter();

        for(auto* feature : class_->getFeatures()){
            if(dynamic_cast<FeatureMethod*>(feature) != nullptr) continue;

            auto* attr = (FeatureAttribute*) feature;
            auto type = attr->getType();

            objectsTable->add(attr->getName(), new std::string(type));
        }

        std::string className = class_->getName();

        if(className == OBJECT_CLASS->getName()) return;

        // std::string inheritsRaw = class_->getInherits().empty() ? OBJECT_CLASS->getName() : class_->getInherits();
        std::string inherits = getParentType(className);
        Class* superClass = classLookups[inherits];

        buildAttributeScopes(superClass);
    }

    void SemanticAnalyzer::processAttribute(Class *class_, FeatureAttribute *attr) {
        bool isInheritAttr = false;

        for(AST* feature : class_->getFeatures()){
            if(dynamic_cast<FeatureMethod*>(feature) != nullptr) continue;

            auto* innerAttr = (FeatureAttribute*) feature;

            if(innerAttr->getName() == attr->getName()) isInheritAttr = true;
        }

        if(isInheritAttr){
            fail(fmt::format("{}'s attribute [{}] is an attribute of an inherited class.",
                             currentClassName, attr->getName()));
        }

        std::string inheritType = getParentType(class_->getName());
        if(inheritType == "_no_type") return;

        Class* superClassDef = classLookups[inheritType];
        processAttribute(superClassDef, attr);
    }

    void SemanticAnalyzer::processMethod(Class *class_, FeatureMethod *method, FeatureMethod *parentMethod) {
        if(parentMethod == nullptr) return;

        std::vector<Formal*> methodFormals = method->getFormalArguments();
        std::vector<Formal*> parentMethodFormals = parentMethod->getFormalArguments();

        if(method->getReturnType() != parentMethod->getReturnType()){
            std::string message = fmt::format("{}: In redefined method {}<{}> differs from the ancestor method return type {}<{}>",
                                              class_->getName(), method->getName(), method->getReturnType(),
                                              parentMethod->getName(), parentMethod->getReturnType());
            fail(message);
        }

        int currentIndex = 0;
        while(currentIndex < methodFormals.size() &&
                currentIndex < parentMethodFormals.size()){
            Formal* methodArg = methodFormals[currentIndex];
            Formal* parentMethodArg = parentMethodFormals[currentIndex];

            if(methodArg->getType() != parentMethodArg->getType()){
                std::string message = fmt::format("{}: In redefined method {}, the return type of argument [{}]<{}> differs from the corresponding ancestor method argument return type [{}]<{}>.",
                                                  class_->getName(), method->getName(), methodArg->getName(), methodArg->getType(),
                                                  parentMethodArg->getName(), parentMethodArg->getType());
                fail(message);
            }

            currentIndex++;
        }

        std::string parentClassType = getParentType(class_->getName());

        if(parentClassType == "_no_type")
            return;

        Class* newParentClass = classLookups[parentClassType];
        FeatureMethod* newParentMethod = nullptr;

        for(auto* feature : newParentClass->getFeatures()){
            if(dynamic_cast<FeatureAttribute*>(feature) != nullptr) continue;

            auto* featureMethod = (FeatureMethod*) feature;

            if(featureMethod->getName() == method->getName()){
                newParentMethod = featureMethod;
                break;
            }
        }

        processMethod(newParentClass, method, newParentMethod);
    }

    SymbolTable<std::string, std::string> *SemanticAnalyzer::getObjectsTable() const {
        return objectsTable;
    }

    std::string SemanticAnalyzer::getCurrentClassName() const {
        return currentClassName;
    }

    FeatureMethod* SemanticAnalyzer::lookupMethodInChain(const std::string &className, const std::string &method) {
        if(className == "_no_type") return nullptr;

        auto* definition = classLookups[className]->getMethod(method);

        if(definition != nullptr) return definition;

        std::string parentClass = getParentType(className);

        return lookupMethodInChain(parentClass, method);
    }

    FeatureMethod *SemanticAnalyzer::lookupMethod(const std::string &className, const std::string &method) {
        auto* chainMethod = lookupMethodInChain(className, method);

        if(chainMethod != nullptr) return chainMethod;
        if(isPrimitive(className)){
            return classLookups[className]->getMethod(method);
        }

        return nullptr;
    }

    Class *SemanticAnalyzer::getParentClass(const std::string &type) {
        return classLookups[type];
    }

    void SemanticAnalyzer::setCurrentClassName(const std::string &name) {
        currentClassName = name;
    }

} // CoolCompiler