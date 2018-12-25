#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_
#include "ast.hpp"
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
        using namespace std;
        cerr << "constructing class -- " << Type::M << endl;
        for (auto p: *Type::M) cerr << p.first << ", " << p.second << endl;
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
    : t(t), id(id), str(str) {}
    Type t;
    std::string id;
    bool checked = false;
    const char *str;
    void accept(Visitor *v) { v->visit(this); }
};

struct Param: ParamDecl {
    Param(Type t, std::string id)
    : ParamDecl(t, id, "method parameter") {}
};

struct Var: ParamDecl {
    Var(Type t, std::string id)
    : ParamDecl(t, id, "variable declaration") {}
};

struct Method: Node {
    Method(std::string type, std::string id, std::vector<Param*> pl,
           std::vector<Var*> vl, std::vector<Statement*> sl, Expression *ret)
    : type(type), id(id), pl(pl), vl(vl), sl(sl), ret(ret) {}
    Type type;
    std::string id;
    std::vector<Param*> pl;
    std::vector<Var*> vl;
    std::vector<Statement*> sl;
    Expression *ret;
    void accept(Visitor *v) { v->visit(this); }
    bool operator==(Method& rhs) const {
        if (id != rhs.id || pl.size() != rhs.pl.size())
            return false;

        int idx = 0;
        for (Param *p: pl)
            if (p->t != rhs.pl[idx++]->t)
                return false;
        return true;
    }
    bool operator!=(Method& rhs) const {
        return !(*this == rhs);
    }

    bool checked = false;
};

// typedef std::pair<Type, std::string> ParamDecl;

}

#endif
