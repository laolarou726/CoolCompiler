//
// Created by luoli on 2022/10/26.
//

#ifndef COOLCOMPILER_SCOPE_H
#define COOLCOMPILER_SCOPE_H

#include "ScopeEntry.h"

namespace CoolCompiler {

    template<class Type, class Info>
    class Scope {
    private:
        ScopeEntry<Type, Info>* entry;
        Scope<Type, Info>* value;
    public:
        Scope(ScopeEntry<Type, Info>* entry, Scope* value);
        ScopeEntry<Type, Info>* getEntry() const;
        Scope<Type, Info>* getValue() const;
    };

    template<class Type, class Info>
    Scope<Type, Info>::Scope(ScopeEntry<Type, Info> *entry, Scope *value) {
        this->entry = entry;
        this->value = value;
    }

    template<class Type, class Info>
    ScopeEntry<Type, Info> *Scope<Type, Info>::getEntry() const {
        return entry;
    }

    template<class Type, class Info>
    Scope<Type, Info>* Scope<Type, Info>::getValue() const {
        return value;
    }

} // CoolCompiler

#endif //COOLCOMPILER_SCOPE_H
