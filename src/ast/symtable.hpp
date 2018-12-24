#ifndef SYMTABLE_
#define SYMTABLE_

#include <unordered_map>
#include <unordered_set>
#include <stack>

namespace ast {

template<typename Sym, typename Dat>
struct SymTable {
    using Tab = std::unordered_map<Sym, Dat>;
    using Idset = std::unordered_set<Sym>;

    SymTable() {
        stk.push(Tab());
        id_set.push(Idset());
    }

    int insert(Sym var, Dat dat) {
        if (id_set.top().count(var)) return -1;
        stk.top()[var] = dat;
        id_set.top().insert(var);
        return 0;
    }
    Dat *find(Sym var) {
        if (!stk.top().count(var)) return nullptr;
        return &stk.top()[var]; // return 0 if it's pointer.
    }
    void enter() {
        id_set.push(Idset());
        if (stk.empty()) stk.push(Tab());
        else stk.push(stk.top());
    }
    void leave() {
        stk.pop();
        id_set.pop();
    }
    std::stack<Tab> stk;
    std::stack<Idset> id_set;
};

}

#endif