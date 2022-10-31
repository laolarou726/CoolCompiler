//
// Created by luoli on 2022/9/18.
//

#include "Class.h"
#include "FeatureAttribute.h"

namespace CoolCompiler {
    Class::Class(const std::string &name, const std::vector<AST*> &features, const std::string &inherits) : AST("class") {
        this->name = name;
        this->features = features;
        this->inherits = inherits;
    }

    std::string Class::getName() const {
        return name;
    }

    std::vector<AST*> Class::getFeatures() const {
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
} // CoolCompiler