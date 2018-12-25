#ifndef STATEMENT_HPP_
#define STATEMENT_HPP_

#include <vector>
#include <string>
#include "ast.hpp"

namespace ast {

struct Expression;
struct Statement: public Node {};

struct Block: Statement {
    Block(std::vector<Statement*> sl): sl(sl) {}
    std::vector<Statement*> sl;
    void accept(Visitor *v) { v->visit(this); }
};

struct If: public Statement {
    If(Expression *cond, Statement *i, Statement *e): cond(cond), i(i), e(e) {}
    Expression *cond;
    Statement *i, *e;
    void accept(Visitor *v) { v->visit(this); }
};

struct While: public Statement {
    While(Expression *e, Statement *s): e(e), s(s) {}
    Expression *e;
    Statement *s;
    void accept(Visitor *v) { v->visit(this); }
};

struct Println: public Statement {
    Println(Expression *e): e(e) {}
    Expression *e;
    void accept(Visitor *v) { v->visit(this); }
};

struct Assign: public Statement {
    Assign(Object *o, Expression *e): o(o), e(e) {}
    Object *o;
    Expression *e;
    void accept(Visitor *v) { v->visit(this); }
};

struct ArrAssign: public Statement {
    ArrAssign(Object *o, Expression *idx, Expression *e): o(o), idx(idx), e(e) {}
    Object *o;
    Expression *e, *idx;
    void accept(Visitor *v) { v->visit(this); }
};

struct Return: Statement {
    Return(Expression *e): e(e) {}
    Expression *e;
    void accept(Visitor *v) { v->visit(this); }
};

}

#endif