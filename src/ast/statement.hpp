#ifndef STATEMENT_HPP_
#define STATEMENT_HPP_

#include <vector>
#include "ast.hpp"

namespace ast {

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

// TODO: assign

}

#endif