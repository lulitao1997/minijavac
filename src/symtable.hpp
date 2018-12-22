#ifndef SYMTABLE_
#define SYMTABLE_

#include <string>

struct SymTable {

    void insert(std::string var);
    bool find(std::string var);
    void enter();
    void leave();

};

#endif