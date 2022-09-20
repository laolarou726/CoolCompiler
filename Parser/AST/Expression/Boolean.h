//
// Created by luoli on 2022/9/18.
//

#ifndef COOLCOMPILER_BOOLEAN_H
#define COOLCOMPILER_BOOLEAN_H

#include "Expression.h"

namespace CoolCompiler {

    class Boolean : Expression {
    private:
        bool value;
    public:
        explicit Boolean(bool value);
        bool getValue() const;
    };

} // CoolCompiler

#endif //COOLCOMPILER_BOOLEAN_H
