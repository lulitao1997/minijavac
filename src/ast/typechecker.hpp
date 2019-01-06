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
            parent->type = Type("<error>");
        }
    }

    ///////////// Expressions ////////////////
    void visit(IntConst *o) { o->type = Type("int"); }
    void visit(BoolConst *o) { o->type = Type("boolean"); }
    void visit(Object *o) {
        auto ptr = env.find(o->id);
        // std::cerr << " >>>>> find variable type: " << o->id << ", " << *ptr << std::endl;
        if (!ptr) {
            complain(o->loc) << "variable `" << o->id << "` undefined\n";
            return;
        }
        o->type = *ptr;
        if (!o->type)
            complain(o->loc) << "type `" << o->type << "` undefined\n";
    }

    void visit(Uop *o) {
        switch(o->op) {
            case Not:
                o->type = Type("boolean");
                // TASSERT(o->e, Type("boolean"), "expression after `!` must be bool or int", o);  // bool <= int
                o->e->accept(this);
                if (!o->e->type.is_arithmetic()) {
                    complain(o->loc) << "expression after `!` must be bool or int\n";
                    o->type = Type("<error>");
                }
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
                assert(o->op && 0);
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
                o->type = Type("<error>");
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
        if (!o->e->type) return;

        bool error = false;

        for (Expression *e: o->param_list)
            e->accept(this);

        std::vector<Method*> match = {};
        for (Method *m: o->e->type.methods())
            if (m->is_compatible(o))
                match.push_back(m);

        if (match.empty()) {
            complain(o->loc) << "cannot find method matching dispatch `" << *o << "`\n";
            return;
        }
        if (match.size() > 1) {
            auto &a = (complain(o->loc) << "ambiguous dispatch, candidates:\n");
            for (auto m: match)
                a << "\t" << m->loc << ": " << *m << "\n";
        }
    }
    void visit(NewObj *o) {
        if (!o->type) {
            complain(o->loc) << "type `" << o->type << "` undefined in new statement\n";
        }
    }
    void visit(NewArr *o) {
        if (!o->type) {
            complain(o->loc) << "type `" << o->type << "` undefined in new statement\n";
        }
        o->idx->accept(this);
        if (o->idx->type != Type("int"))
            complain(o->idx->loc) << "array subscript must be integer\n";
    }

    ///////// statement /////////
    void visit(Block *o) {
        env.enter();
        for (Statement *p: o->sl)
            p->accept(this);
        env.leave();
    }
    void visit(If *o) {
        o->cond->accept(this);
        if (!o->cond->type.is_compatible(Type("boolean")))
            complain(o->cond->loc) << "condition of if is not compatible with boolean\n";
        o->i->accept(this);
        if (o->e) o->e->accept(this);
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
            complain(o->loc) << "type not compatible, lhs type: "
                             << o->o->type << ", rhs type: " << o->e->type << std::endl;
        }
    }
    void visit(ArrAssign *o) {
        o->o->accept(this);
        o->e->accept(this);
        o->idx->accept(this);
        if (!o->o->type.is_array()) {
            complain(o->o->loc) << "object `" << o->o->id << "` is not an array\n";
            return;
        }
        if (o->idx->type != Type("int")) {
            complain(o->idx->loc) << "array subscript is not int\n";
            return;
        }
        if (!o->o->type.array_body().is_compatible(o->e->type)) {
            complain(o->o->loc) << "type not compatible, lhs type: "
                                << o->o->type.array_body() << ", rhs type: " << o->e->type << std::endl;
            return;
        }
    }
    void visit(Return *o) {
        o->e->accept(this);
        Type t = *env.find("<return>");
        if (o->e->type != t)
            complain(o->loc) << "return type `" << o->e->type << "` not compatible with `" << t << std::endl;
    }

    ///////// Class ///////////
    void visit(Program *o) {
        std::unordered_set<std::string> cid;
        bool has_circle = false;
        for (Class *c: o->cl) {
            Class *t = c;
            while (c->parent != Type("<object>")) {
                // std::cerr << " >>> " << c->id << "::" << c << ", " << c->get_parent() << ", " << c->parent << std::endl;
                c = c->get_parent();
                if (c == t) {
                    complain(c->loc) << "circular dependency detected\n";
                    return;
                }
            }
        }
        for (Class *c: o->cl) {
            if (cid.count(c->id))
                complain(c->loc) << "duplicate definition of `" << c->id << "`\n";
            if (!c->inherited)
                c->inherit();
        }

        for (Class *c: o->cl) {
            c->accept(this);
        }
    }
    void visit(Class *o) {
        env.enter();
        // param and variable cannot be defined as `this`.
        env.insert("this", Type(o->id));

        if (!o->parent)
            complain(o->loc)
                << "class `" << o->id << "` inherit from undefined type `" << o->parent << "`\n";
        for (Var *p: o->attrs) {
            p->accept(this);
        }
        for (Method *m: o->methods) {
            m->accept(this);
        }
        env.leave();
    }
    void visit(Method *m) {
        // don't complain if checked. will be check twice if in the Base class;
        if (m->checked) return;
        m->checked = true;
        env.enter();
        // allow param shadowing attr, but we do not allow param have same id.
        if (!m->type) {
            complain(m->loc) << "method return type undefined\n";
            return;
        }

        for (Param *p: m->pl)
            p->accept(this);

        env.enter();
        env.insert("<return>", m->type);
        for (Statement *s: m->sl)
            s->accept(this);
        env.leave();

        env.leave();
    }
    void check(ParamDecl *p) {
        // don't complain if checked. will be check twice if in the Base class;
        if (p->checked) return;
        p->checked = true;
        if (!p->type)
            complain(p->loc) << "type `" << p->type << "` undefined in " << p->str << std::endl;
        if (env.insert(p->id, p->type) == -1)
            complain(p->loc) << "dulplicate definition of `" << p->id << "` in " << p->str << std::endl;
    }
    void visit(Param *p) { check(p); }
    void visit(Var *p) { check(p); }
};

}

#endif
