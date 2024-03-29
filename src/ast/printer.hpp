#ifndef PRINTER_HPP_
#define PRINTER_HPP_

#include <iostream>
#include <string>
#include "expression.hpp"
#include "statement.hpp"
#include "program.hpp"

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
        out << idnt_str << "NewObj: " << o->type << std::endl;
    }
    void visit(NewArr *o) {
        out << idnt_str << "NewArr " << o->type << std::endl;
        depth++;
        o->idx->accept(this);
        depth--;
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
        if (s->e) s->e->accept(this);
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
    void visit(ArrAssign *s) {
        out << idnt_str << "Assign: " << std::endl;
        depth++;
        s->o->accept(this);
        // out << idnt_str << s->id << std::endl;
        s->idx->accept(this);
        s->e->accept(this);
        depth--;
    }
    void visit(Assign *s) {
        out << idnt_str << "Assign: " << std::endl;
        depth++;
        s->o->accept(this);
        s->e->accept(this);
        depth--;
    }
    void visit(Return *s) {
        out << idnt_str << "Return: " << std::endl;
        depth++;
        s->e->accept(this);
        depth--;
    }

    /////// class /////////
    void visit(Program *s) {
        out << idnt_str << "Program: " << std::endl;
        depth++;
        for (Class *c: s->cl)
            c->accept(this);
        depth--;
    }
    void visit(Class *s) {
        out << idnt_str << "Class: " << s->id << ": " << s->parent << std::endl;
        depth++;
        for (auto a: s->attrs)
            out << idnt_str << "attrs: " << a->type << " : " << a->id << std::endl;
        for (Method *a: s->methods)
            a->accept(this);
        depth--;
    }

    void visit(Method *s) {
        out << idnt_str << "Method: " << s->id << " : " << s->type << std::endl;
        depth++;
        for (auto p: s->pl)
            p->accept(this);
        // for (auto p: s->vl)
        //     { out << idnt_str << "var_decl: "; p->accept(this); }
        for (auto p: s->sl)
            p->accept(this);
        depth--;
    }

    void visit(Var *p) {
        out << idnt_str << "VarDecl: " << p->type << ", " << p->id << std::endl;
    }
    void visit(Param *p) {
        out << idnt_str << "Param: " << p->type << ", " << p->id << std::endl;
    }

    // void visit(Var *s) {
    //     out << idnt_str << "Var: " <<
    // }
    #undef idnt_str
    #undef PConst
};

}

#endif
