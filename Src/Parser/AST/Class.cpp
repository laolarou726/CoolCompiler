//
// Created by luoli on 2022/9/18.
//

#include "Class.h"
#include "Feature/FeatureAttribute.h"
#include "../../Semantic/SemanticAnalyzer.h"

namespace CoolCompiler {
    Class::Class(const std::string &name, const std::vector<FeatureBase*> &features, const std::string &inherits) : AST("class") {
        this->name = name;
        this->features = features;
        this->inherits = inherits;
    }

    std::string Class::getName() const {
        return name;
    }

    std::vector<FeatureBase*> Class::getFeatures() const {
        return features;
    }

    std::string Class::getInherits() const {
        return inherits;
    }

    FeatureMethod* Class::getMethod(const std::string &method) const {
        for(auto* feature : features){
            if(typeid(feature) == typeid(FeatureAttribute)) continue;

            auto* featureMethod = (FeatureMethod*) feature;

            if(featureMethod->getName() == method) return featureMethod;
        }

        return nullptr;
    }

    std::string Class::typeCheck(SemanticAnalyzer *analyzer) {
        analyzer->setCurrentClassName(name);
        analyzer->ensureAttributesUnique(this);

        auto* objectsTable = analyzer->getObjectsTable();
        std::string currentName = analyzer->getCurrentClassName();

        objectsTable->enter();
        objectsTable->add("self", &currentName);

        analyzer->buildAttributeScopes(this);

        for(auto* feature : features){
            if(typeid(feature) == typeid(FeatureMethod)){
                auto* featureMethod = (FeatureMethod*) feature;
                analyzer->processMethod(this, featureMethod, featureMethod);
            }

            if(typeid(feature) == typeid(FeatureAttribute)){
                std::string parentType = analyzer->getParentType(analyzer->getCurrentClassName());
                analyzer->processAttribute(analyzer->getParentClass(parentType), (FeatureAttribute*) feature);
            }
        }

        for(auto* feature : features){
            feature->typeCheck(analyzer);
        }

        objectsTable->exit();
    }
} // CoolCompiler