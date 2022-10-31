//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_CLASS_H
#define COOLCOMPILER_CLASS_H

#include <vector>
#include <iostream>
#include "AST.h"
#include "FeatureMethod.h"

namespace CoolCompiler {

    class Class : public AST {
    private:
        std::string name;
        std::vector<AST*> features;
        std::string inherits;
    public:
        Class(const std::string &name, const std::vector<AST*> &features, const std::string &inherits);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::vector<AST*> getFeatures() const;
        [[nodiscard]] std::string getInherits() const;
        [[nodiscard]] FeatureMethod* getMethod(const std::string &method) const;

        void print(int depth) override{
            printTab(depth);
            std::cout << "CLASS [" << name << "]:["  << inherits << "][" << std::endl;

            for(AST *ast : features)
                ast->print(depth + 1);

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_CLASS_H
