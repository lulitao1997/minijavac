#ifndef PRINTER_HPP_
#define PRINTER_HPP_

#include <iostream>
#include <string>
#include "expression.hpp"
#include "statement.hpp"

namespace ast {

struct Printer: public Visitor {
    Printer(std::ostream& out): out(out) {};
    std::ostream &out;
    int depth=0;
    const int indent = 2;

    #define idnt_str std::string(depth*indent, ' ')

    #define PConst(name) virtual void visit(name *o) { \
        out << idnt_str << #name << ": " << o->val << std::endl; \
    } \

    ////////////// expression //////////////////
    PConst(IntConst);
    PConst(BoolConst);

    void visit(Bop *o) {
        out << idnt_str << "bop: " << (char)o->op << std::endl;
        depth++;
        o->l->accept(this);
        o->r->accept(this);
        depth--;
    }

    void visit(Uop *o) {
        out << idnt_str << "uop: " << (char)o->op << std::endl;
        depth++;
        o->e->accept(this);
        depth--;
    }
    void visit(Dispatch *o) {
        out << idnt_str << "Dispatch: " << std::endl;
        depth++;
        o->e->accept(this);
        out << idnt_str << o->id << std::endl;
        for (Expression *t: o->param_list) {
            t->accept(this);
        }
        depth--;
    }

    void visit(NewObj *o) {
        out << idnt_str << "NewObj: " << o->id << std::endl;
    }
    void visit(Object *o) {
        out << idnt_str << "Object: " << o->id << std::endl;
    }


    ///////////////// statements //////////////
    void visit(Block *s) {
        out << idnt_str << "Block: " << std::endl;
        depth++;
        for (auto t: s->sl)
            t->accept(this);
        depth--;
    }
    void visit(If *s) {
        out << idnt_str << "If: " << std::endl;
        depth++;
        s->cond->accept(this);
        s->i->accept(this);
        s->e->accept(this);
        depth--;
    }
    void visit(While *s) {
        out << idnt_str << "While: " << std::endl;
        depth++;
        s->e->accept(this);
        s->s->accept(this);
        depth--;
    }
    void visit(Println *s) {
        out << idnt_str << "Println: " << std::endl;
        depth++;
        s->e->accept(this);
        depth--;
    }

    #undef idnt_str
    #undef PConst
};

}

#endif