#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_
#include "ast.hpp"
#include "statement.hpp"
#include "type.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>

namespace ast {

struct Program: Node {
    Program(std::vector<Class*> cl): cl(cl) {}
    std::vector<Class*> cl;

    void accept(Visitor *v) { v->visit(this); }
};

struct Class: Node {
    Class(std::string id, std::string parent, std::vector<Var*> attrs, std::vector<Method*> methods)
    : id(id), parent(parent), attrs(attrs), methods(methods)
    {
        (*Type::M)[id] = this;
    }

    std::string id;
    Type parent;
    std::vector<Var*> attrs;
    std::vector<Method*> methods;
    void accept(Visitor *v) { v->visit(this); }

protected:
    friend class TypeChecker;
    bool inherited = false;
    void inherit();
    Class *get_parent();
};

struct ParamDecl: Node {
    ParamDecl(Type t, std::string id, const char *str)
    : type(t), id(id), str(str) {}
    Type type;
    std::string id;
    bool checked = false;
    const char *str;
};

struct Param: ParamDecl {
    Param(Type t, std::string id)
    : ParamDecl(t, id, "method parameter") {}
    void accept(Visitor *v) { v->visit(this); }
};

struct Var: ParamDecl, Statement {
    Var(Type t, std::string id)
    : ParamDecl(t, id, "variable declaration") {}
    void accept(Visitor *v) { v->visit(this); }
};

struct Method: Node {
    Method(std::string type, std::string id, std::vector<Param*> pl,
           std::vector<Statement*> sl)
    : type(type), id(id), pl(pl), sl(sl) {}
    Type type; // return type
    std::string id;
    std::vector<Param*> pl;
    // std::vector<Var*> vl;
    std::vector<Statement*> sl;
    void accept(Visitor *v) { v->visit(this); }
    bool operator==(Method& rhs) const;
    bool operator!=(Method& rhs) const {
        return !operator==(rhs);
    }
    friend std::ostream& operator<<(std::ostream& out, Method& rhs);
    bool is_compatible(Dispatch *o) const;
    bool checked = false;
};

}

#endif
