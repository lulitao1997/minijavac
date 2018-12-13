#ifndef AST_HPP
#define AST_HPP

#include <vector>

namespace ast {
    struct Expression;
    struct IntConst;
    struct BoolConst;
    struct Object;
    struct Uop;
    struct NewObj;
    struct Dispatch;
    struct Bop;

    struct Statement;
    struct Block;
    struct If;
    struct While;
    struct Println;
}


namespace ast {

struct Visitor {
    // Visitor() = delete;
    #define V0(name) virtual void visit(name *o) = 0
    V0(IntConst);
    V0(BoolConst);
    V0(Object);
    V0(Uop);
    V0(NewObj);
    V0(Dispatch);
    V0(Bop);

    V0(Block);
    V0(If);
    V0(While);
    V0(Println);
    #undef V0
};
struct Node {
    virtual void accept(Visitor *v) = 0;
};

template<typename Tp>
std::vector<Tp> append(std::vector<Tp> L, Tp v) {
    L.push_back(v);
    return L;
}
template<typename Tp>
std::vector<Tp> single(Tp v) {
    return std::vector<Tp>{v};
}


typedef Expression *expr_ptr;

}

#endif