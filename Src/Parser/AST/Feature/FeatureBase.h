//
// Created by luoli on 2022/10/30.
//

#ifndef COOLCOMPILER_FEATUREBASE_H
#define COOLCOMPILER_FEATUREBASE_H

#include <string>
#include "../AST.h"

namespace CoolCompiler {

    class SemanticAnalyzer;

    class FeatureBase : public AST{
    public:
        explicit FeatureBase(const std::string &identifier);
        void print(int depth) override = 0;
        virtual std::string typeCheck(SemanticAnalyzer* analyzer) = 0;
    };

} // CoolCompiler

#endif //COOLCOMPILER_FEATUREBASE_H
