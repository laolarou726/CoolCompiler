//
// Created by luoli on 2022/10/26.
//

#ifndef COOLCOMPILER_SCOPEENTRY_H
#define COOLCOMPILER_SCOPEENTRY_H

#include "string"

namespace CoolCompiler {

    template<class Type, class Info>
    class ScopeEntry {
    private:
        Type type;
        Info* info;
    public:
        ScopeEntry(const Type &type, Info* info);
        Type getType() const;
        Info* getInfo() const;
    };

    template<class Type, class Info>
    ScopeEntry<Type, Info>::ScopeEntry(const Type &type, Info *info) {
        this->type = type;
        this->info = info;
    }

    template<class Type, class Info>
    Type ScopeEntry<Type, Info>::getType() const {
        return type;
    }

    template<class Type, class Info>
    Info* ScopeEntry<Type, Info>::getInfo() const {
        return info;
    }

} // CoolCompiler

#endif //COOLCOMPILER_SCOPEENTRY_H
