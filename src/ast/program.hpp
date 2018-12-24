#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_
#include "ast.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>

namespace ast {

struct Param;
struct Var;
struct Method;

struct Class;

struct Type {
    explicit Type(std::string name);
    Type(): Type("<error>") {}

    // bool operator<=(const Type& rhs);
    bool is_compatible(const Type &rhs) const;
    bool is_arithmetic() const;
    bool operator==(const Type& rhs) const;
    bool operator!=(const Type& rhs) const;

    explicit operator bool() const;
    friend std::ostream& operator<<(std::ostream& out, const Type& t) {
        return out << *t.id;
    }
    // Class& operator*() {
    //     assert(*c);
    //     return **c;
    // }
    // Class *operator->() {
    //     assert(*c);
    //     return *c;
    // }
    // std::string id;
    bool is_array() const ;
    Type array_body() const ;
    const std::vector<Method*>& methods();
    friend class Class;
protected:
    Class **c;
    const std::string *id;
    static std::unordered_map<std::string, Class*> *M;


    Class *make_class(std::string id, std::string parent, std::vector<Var*> attrs, std::vector<Method*> methods) const ;
    std::string remove_subscript(std::string s) const;
};

struct Class: Node {
    Class(std::string id, std::string parent, std::vector<Var*> attrs, std::vector<Method*> methods)
    : id(id), parent(parent), attrs(attrs), methods(methods)
    {
        using namespace std;
        cerr << "constructing class -- " << Type::M << endl;
        for (auto p: *Type::M) cerr << p.first << ", " << p.second << endl;
        (*Type::M)[id] = this;
    }

    std::string id;
    Type parent;
    // std::vector<Var*> attrs;
    std::vector<Var*> attrs;
    std::vector<Method*> methods;
    void accept(Visitor *v) { v->visit(this); }

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
};

// typedef std::pair<Type, std::string> ParamDecl;
struct ParamDecl: Node {
    ParamDecl(Type t, std::string id, const char *str)
    : t(t), id(id), str(str) {}
    Type t;
    std::string id;
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

}

#endif
