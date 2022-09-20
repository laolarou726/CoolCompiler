//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_CLASS_H
#define COOLCOMPILER_CLASS_H

#include <vector>
#include "AST.h"

namespace CoolCompiler {

    class Class : AST {
    private:
        std::string name;
        std::vector<AST> features;
        std::string inherits;
    public:
        Class(const std::string &name, const std::vector<AST> &features, const std::string &inherits);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] std::vector<AST> getFeatures() const;
        [[nodiscard]] std::string getInherits() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_CLASS_H
