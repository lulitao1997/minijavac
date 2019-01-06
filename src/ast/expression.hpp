#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

#include "ast.hpp"
#include "program.hpp"
#include <string>
#include <vector>

namespace ast {

struct Expression: public Node {
    Type type;
};

struct IntConst: Expression {
    IntConst(int val): val(val) {}
    int val;
    void accept(Visitor *v) { v->visit(this); }
};
struct BoolConst: Expression {
    BoolConst(bool val): val(val) {}
    bool val;
    void accept(Visitor *v) { v->visit(this); }
};

struct Object: public Expression {
    Object(std::string id): id(id) {}
    std::string id;
    void accept(Visitor *v) { v->visit(this); }
};

enum OpType {
    And = '&',
    Le = '<',
    Add = '+',
    Minus = '-',
    Mul = '*',
    Arr = '[',
    Len = 'L',
    // NewInt = 'N',
    Not = '!',
    Bracket = '('
};

// template<int Op>
struct Uop: public Expression {
    Uop(OpType op, Expression *e): op(op), e(e) {}
    OpType op;
    Expression *e;
    void accept(Visitor *v) { v->visit(this); }
};

struct Bop: public Expression {
    Bop(OpType op, Expression *l, Expression *r): op(op), l(l), r(r) {}
    OpType op;
    Expression *l, *r;
    void accept(Visitor *v) { v->visit(this); }
};

struct Dispatch: public Expression {
    Dispatch(Expression *e, std::string id, std::vector<Expression*> param_list): e(e), id(id), param_list(param_list) {}
    Expression *e;
    std::string id;
    std::vector<Expression*> param_list;
    friend std::ostream& operator<<(std::ostream& out, Dispatch& rhs);
    void accept(Visitor *v) { v->visit(this); }
};

struct NewObj: public Expression {
    NewObj(Type t) { type = t; }
    // Type type;
    void accept(Visitor *v) { v->visit(this); }
};

struct NewArr: public Expression {
    NewArr(Type t, Expression *idx):idx(idx) { type = t; }
    Expression *idx;
    void accept(Visitor *v) { v->visit(this); }
};

}

#endif
