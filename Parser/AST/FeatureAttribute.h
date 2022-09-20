//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_FEATUREATTRIBUTE_H
#define COOLCOMPILER_FEATUREATTRIBUTE_H

#include "AST.h"
#include "Expression/Expression.h"

namespace CoolCompiler {

    class FeatureAttribute : AST {
    private:
        std::string name;
        std::string type;
        Expression init;
    public:
        FeatureAttribute(const std::string &name, const std::string &type,
                         const Expression &init);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::string getType() const;
        [[nodiscard]] Expression getInit() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_FEATUREATTRIBUTE_H
