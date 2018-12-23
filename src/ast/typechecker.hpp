#ifndef TYPECHECKER_HPP_
#define TYPECHECKER_HPP_

#include <cassert>
#include "expression.hpp"
#include "statement.hpp"
#include "program.hpp"
#include "symtable.hpp"

namespace ast {

struct TypeChecker: Visitor {
    // expressions
    // std::string ret_type;
    Env env;
    std::unordered_map<std::string, std::unordered_map<std::string, Method *>>& method_tab;

    void TASSERT(Expression *expr, std::string type, std::string msg, Expression *parent) {
        expr->accept(this);
        if (!leq(expr->type, type)) {
            report_error(expr->loc, msg);
        }
    }

    void visit(IntConst *o) { o->type = "int"; }
    void visit(BoolConst *o) { o->type = "bool"; }
    void visit(Object *o) { env.find(o->id); } // TODO }

    void visit(Uop *o) {
        switch(o->op) {
            case NewInt:
                o->type = "int[]";
                TASSERT(o->e, "int", "size of array is not int.", o);
                break;
            case Not:
                o->type = "bool";
                TASSERT(o->e, "int", "expression after `!` must be bool or int", o);  // bool <= int
                break;
            case Bracket:
                o->e->accept(this);
                o->type = o->e->type;
                break;
            default:
                assert(0);
        }

    }
    void visit(Bop *o) {
        if (o->op != Arr) {
            o->l->accept(this);
            o->type = o->l->type;
            TASSERT(o->r, o->l->type, "type not compatible", o);
            // ret type as-is
        }
        else if (o->op == Arr) {
            o->l->accept(this);
            o->type = o->l->type;
            TASSERT(o->r, "int", "array subscript is not int", o);
        }
    }
    void visit(Dispatch *o) {
        o->e->accept(this);
        o->type = method_tab[o->e->type][o->id]->type;

        for (Expression *e: o->param_list) {

        }
    }
};

}

#endif
