#ifndef TYPECHECKER_HPP_
#define TYPECHECKER_HPP_

#include <cassert>
#include <iostream>

#include <unordered_set>
#include "expression.hpp"
#include "statement.hpp"
#include "program.hpp"
#include "symtable.hpp"

namespace ast {

// calculate every expression's type, check if, while stmt;
struct TypeChecker: Visitor {
    // expressions
    // std::string ret_type;
    SymTable<std::string, Type> env;

    void TASSERT(Expression *expr, Type type, std::string msg, Expression *parent) {
        expr->accept(this);
        if (!(expr->type == type)) {
            complain(expr->loc) << msg << std::endl; ;
        }
    }

    ///////////// Expressions ////////////////
    void visit(IntConst *o) { o->type = Type("int"); }
    void visit(BoolConst *o) { o->type = Type("boolean"); }
    void visit(Object *o) {
        auto ptr = env.find(o->id);
        std::cerr << " >>>>> find variable type: " << o->id << ", " << *ptr << std::endl;
        if (!ptr) {
            complain(o->loc) << "variable undefined\n";
            return;
        }
        o->type = *ptr;
        if (!o->type)
            complain(o->loc) << "type undefined\n";
    }

    void visit(Uop *o) {
        switch(o->op) {
            case NewInt:
                o->type = Type("int[]");
                TASSERT(o->e, Type("int"), "size of array is not int.", o);
                break;
            case Not:
                o->type = Type("boolean");
                TASSERT(o->e, Type("boolean"), "expression after `!` must be bool or int", o);  // bool <= int
                break;
            case Bracket:
                o->e->accept(this);
                o->type = o->e->type;
                break;
            case Len:
                o->e->accept(this);
                o->type = Type("int");
                if (!o->e->type.is_array() && o->e->type != Type("String"))
                    complain(o->loc) << "object does not support .length method\n";
                break;
            default:
                std::cerr << " error op type:::: " << o->op << std::endl;
                assert(0);
        }

    }
    void visit(Bop *o) {
        if (o->op != Arr) {
            o->l->accept(this);
            o->r->accept(this);
            o->type = o->l->type;
            // TASSERT(o->r, o->l->type, "type not compatible", o); // TODO: base & derived bop
            if (o->l->type != o->r->type && !(o->l->type.is_arithmetic() && o->r->type.is_arithmetic())) {
                complain(o->loc) << "type not compatible around `" << char(o->op)
                                 << "`, lhs type: " << o->l->type << ", rhs type: " << o->r->type << std::endl;
            }
            // ret type as-is
        }
        else if (o->op == Arr) {
            o->l->accept(this);
            if (o->l->type.is_array())
                o->type = o->l->type.array_body();
            else o->type = Type("<error>");
            TASSERT(o->r, Type("int"), "array subscript is not int", o);
        }
        else assert(0);
    }
    void visit(Dispatch *o) {
        o->e->accept(this);

        bool error = false;

        for (Expression *e: o->param_list)
            e->accept(this);

        Method *sig = nullptr;
        for (Method *m: o->e->type.methods())
            if (m->id == o->id && m->pl.size() == o->param_list.size()) {
                bool eq = true;
                int idx = 0;
                for (ParamDecl *p: m->pl)
                    if (p->t != o->param_list[idx++]->type) {
                        eq = false;
                        break;
                    }
                sig = m;
            }
        if (!sig) {
            // complain(o->loc) << "method undefined\n";
            complain(o->loc);
            o->type = Type("<error>");
            return;
        }
        o->type = sig->type;
    }
    void visit(NewObj *o) {}

    ///////// statement /////////
    void visit(Block *o) {
        for (Statement *p: o->sl)
            p->accept(this);
    }
    void visit(If *o) {
        o->cond->accept(this);
        if (!o->cond->type.is_compatible(Type("boolean")))
            complain(o->cond->loc) << "condition of if is not compatible with boolean\n";
        o->i->accept(this);
        o->e->accept(this);
    }
    void visit(While *o) {
        o->e->accept(this);
        if (!o->e->type.is_compatible(Type("boolean")))
            complain(o->e->loc) << "condition of while is not compatible with boolean\n";
        o->s->accept(this);
    }
    void visit(Println *o) {
        o->e->accept(this);
        if (!o->e->type.is_arithmetic())
            complain(o->e->loc) << "print statement can only accept integer or boolean\n";
    }
    void visit(Assign *o) {
        o->o->accept(this);
        o->e->accept(this);
        if (!o->o->type.is_compatible(o->e->type)) {
            complain(o->e->loc) << "type not compatible, lhs type: "
                                << o->o->type << ", rhs type: " << o->e->type << std::endl;
        }
    }
    void visit(ArrAssign *o) {
        o->o->accept(this);
        o->e->accept(this);
        o->idx->accept(this);
        if (!o->o->type.is_array()) {
            complain(o->o->loc) << "object is not an array\n";
            return;
        }
        if (o->idx->type != Type("int")) {
            complain(o->o->loc) << "array subscript is not int\n";
            return;
        }
        if (!o->o->type.array_body().is_compatible(o->e->type)) {
            complain(o->o->loc) << "type not compatible\n";
            return;
        }
    }

    ///////// Class ///////////
    void visit(Class *o) {
        env.enter();

        // SymTable<std::string, Method*> method_tab;

        for (ParamDecl *p: o->attrs) {
            p->accept(this);
        }
        for (Method *m: o->methods) {
            m->accept(this); //TODO: check dulplicate method name.
        }
        env.leave();
    }
    void visit(Method *m) {
        env.enter();
        // allow param shadowing attr, but we do not allow param have same id.

        if (!m->type) {
            complain(m->loc) << "method return type undefined\n";
        }

        for (ParamDecl *p: m->pl)
            p->accept(this);

        env.enter();
        for (ParamDecl *v: m->vl) {
            v->accept(this);
            // if (v->t == Type("<error>"))
            //     complain(v->loc, "variable type undefined");
            // if (env.insert(v->id, v->t) == -1)
            //     complain(v->loc, "dulplicate variable decleration");
        }

        for (Statement *s: m->sl) {
            s->accept(this);
        }

        env.leave();

        env.leave();
    }
    void visit(ParamDecl *p) {
        if (!p->t)
            complain(p->loc) << p->str << " undefined\n";
        if (env.insert(p->id, p->t) == -1)
            complain(p->loc) << "dulplicate " << p->str << std::endl;
    }
};

}

#endif
