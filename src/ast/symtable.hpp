#ifndef SYMTABLE_
#define SYMTABLE_

#include <unordered_map>
#include <stack>

namespace ast {

template<typename Sym, typename Dat>
struct SymTable {
    using Tab = std::unordered_map<Sym, Dat>;
    void insert(Sym var, Dat dat) {
        stk.top()[var] = dat;
    }
    Dat find(Sym var) {
        return stk.top()[var]; // return 0 if it's pointer.
    }
    void enter() {
        if (stk.empty()) stk.push(Tab{});
        else stk.push(stk.top());
    }
    void leave() {
        stk.pop();
    }
    std::stack<Tab> stk;
};

using Env = SymTable<std::string, std::string>;

}

#endif