//
// Created by luoli on 2022/10/22.
//

#ifndef COOLCOMPILER_SYMBOLTABLE_H
#define COOLCOMPILER_SYMBOLTABLE_H

#include <iostream>
#include "Scope.h"
#include "List.h"

namespace CoolCompiler {

    template<class Type, class Info>
    class SymbolTable {
    private:
        List<Scope<Type, Info>>* scopeList;
    public:
        void enter();
        void exit();
        ScopeEntry<Type, Info>* add(Type type, Info* info);
        Info* lookup(const Type &type);
        Info* probe(const Type &type);
        void fatal(const std::string &message) const;
    };

    template<class Type, class Info>
    void SymbolTable<Type, Info>::enter() {
        scopeList = new List<Scope<Type, Info>>(nullptr, scopeList);
    }

    template<class Type, class Info>
    void SymbolTable<Type, Info>::exit() {
        if(scopeList == nullptr)
            fatal("exit: Can't remove scope from an empty symbol table.");

        scopeList = scopeList->getNext();
    }

    template<class Type, class Info>
    ScopeEntry<Type, Info> *SymbolTable<Type, Info>::add(Type type, Info *info) {
        if(scopeList == nullptr)
            fatal("add: Can't add a symbol without a scope.");

        auto* scopeEntry = new ScopeEntry(type, info);
        scopeList =
                new List<Scope<Type, Info>>(new Scope<Type, Info>(scopeEntry, scopeList->getVal()), scopeList->getNext());

        return scopeEntry;
    }

    template<class Type, class Info>
    Info *SymbolTable<Type, Info>::lookup(const Type &type) {
        auto* currentScopeList = scopeList;

        while(currentScopeList != nullptr){
            auto* currentScope = currentScopeList->getVal();

            while (currentScope != nullptr){
                if(type == currentScope->getEntry()->getType())
                    return currentScope->getEntry()->getInfo();

                currentScope = currentScope->getValue();
            }

            currentScopeList = currentScopeList->getNext();
        }

        return nullptr;
    }

    template<class Type, class Info>
    Info *SymbolTable<Type, Info>::probe(const Type &type) {
        if(scopeList == nullptr)
            fatal("probe: No scope in symbol table.");

        auto* currentScope = scopeList->getVal();
        while(currentScope != nullptr){
            if(type == currentScope->getEntry()->getType())
                return currentScope->getEntry()->getInfo();

            currentScope = currentScope->getValue();
        }

        return nullptr;
    }

    template<class Type, class Info>
    void SymbolTable<Type, Info>::fatal(const std::string &message) const {
        std::cerr << message << std::endl;
    }

} // CoolCompiler

#endif //COOLCOMPILER_SYMBOLTABLE_H
