//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_FEATUREATTRIBUTE_H
#define COOLCOMPILER_FEATUREATTRIBUTE_H

#include "../AST.h"
#include "../Expression/Expression.h"
#include "FeatureBase.h"

namespace CoolCompiler {

    class FeatureAttribute : public FeatureBase {
    private:
        std::string name;
        std::string type;
        Expression* init;
    public:
        FeatureAttribute(const std::string &name, const std::string &type);
        FeatureAttribute(const std::string &name, const std::string &type, Expression* init);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::string getType() const;
        [[nodiscard]] Expression* getInit() const;

        std::string typeCheck(CoolCompiler::SemanticAnalyzer *analyzer) override;

        void print(int depth) override{
            printTab(depth);
            std::cout << "ATTRIBUTE [" << name << "]<"  << type << "> => ";

            if(init != nullptr)
                init->print(depth + 1);
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_FEATUREATTRIBUTE_H
