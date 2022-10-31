//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_PROGRAM_H
#define COOLCOMPILER_PROGRAM_H

#include <vector>
#include "AST.h"
#include "Class.h"

namespace CoolCompiler {

    class Program : public AST {
    private:
        std::vector<Class*>* classes;
    public:
        explicit Program();
        [[nodiscard]] std::vector<Class*>* getClasses() const;

        void print(int depth) override{
            printTab(depth);
            std::cout << "PROGRAM [" << std::endl;

            for(Class *ast : *classes)
                ast->print(depth + 1);

            printTab(depth);
            std::cout << "]" << std::endl;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_PROGRAM_H
