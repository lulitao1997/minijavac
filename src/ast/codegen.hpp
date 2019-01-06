#ifndef CODEGEN_HPP_
#define CODEGEN_HPP_

#include <iostream>
#include "expression.hpp"
#include "statement.hpp"
#include "program.hpp"
#include "symtable.hpp"

namespace ast {

struct CodeGen: Visitor {
    std::ostream out;

    #define O(a, b, c)  out << #a << ' ' << #b << ' ' << #c << std::endl;
    #define T(a, b) out << #a << ' ' << #b << std::endl;
    #define cmt(a)  out << "# " #a << std::endl;

    struct location {

    };

    SymTable<std::string, location> id2loc;
    SymTable<location, std::string> env;

    ////////// expression ///////////
    void visit(IntConst *e) {
        O(mov, %eax, %ebx);
    }
    void visit(BoolConst *e) {

    }
    void visit(Object *e) {

    }
    void visit(Uop *e) {

    }
    void visit(Bop *e) {

    }
    void visit(Dispatch *e) {

    }
    void visit(NewObj *e) {
        if (e->type == Type("int") || e->type == Type("bool")) {

        }
        else {

        }
    }
    void visit(NewArr *e) {

    }
    ////////// statement ////////
    void visit(Block *s) {

    }
    void visit(If *s) {

    }
    void visit(While *s) {

    }

    ////////// class //////////
    void visit(Method *o) {

    }
    void visit(Class *o) {

    }
    void visit(ParamDecl *o) {

    }
    // helper functions.


    #undef O

};

}

#endif