//
// Created by luoli on 2022/10/22.
//

#ifndef COOLCOMPILER_SYMBOLTABLEENTRY_H
#define COOLCOMPILER_SYMBOLTABLEENTRY_H

namespace CoolCompiler {

    template<class Type, class Info>
    class SymbolTableEntry {
    private:
        Type type;
        Info* info;
    public:
        SymbolTableEntry(Type type, Info* info);
        Type getType() const;
        Info* getInfo() const;
    };

    template<class Type, class Info>
    SymbolTableEntry<Type, Info>::SymbolTableEntry(Type type, Info *info) {
        this->type = type;
        this->info = info;
    }

    template<class Type, class Info>
    Type SymbolTableEntry<Type, Info>::getType() const {
        return type;
    }

    template<class Type, class Info>
    Info *SymbolTableEntry<Type, Info>::getInfo() const {
        return info;
    }

} // CoolCompiler

#endif //COOLCOMPILER_SYMBOLTABLEENTRY_H
