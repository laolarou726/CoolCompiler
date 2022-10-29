//
// Created by luoli on 2022/10/26.
//

#ifndef COOLCOMPILER_LIST_H
#define COOLCOMPILER_LIST_H

namespace CoolCompiler {

    template<class T>
    class List {
    private:
        T* val;
        List<T>* next;
    public:
        List(T* val, List<T>* next = nullptr){
            this->val = val;
            this->next = next;
        }

        T* getVal() const{
            return val;
        }

        List<T>* getNext() const{
            return next;
        }

        int length() const{
            List<T>* current = this;
            int result = 0;

            while(current != nullptr){
                result++;
                current = current->getNext();
            }

            return result;
        }
    };

} // CoolCompiler

#endif //COOLCOMPILER_LIST_H
