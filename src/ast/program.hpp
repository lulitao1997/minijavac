#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_
#include "ast.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>

namespace ast {

struct Var;
struct Method;

struct Class;

Class *make_class(std::string id, std::string parent, std::vector<ParamDecl*> attrs, std::vector<Method*> methods);
struct Type {
    explicit Type(std::string id) {
        using namespace std;
        if (!M) {
            M = new unordered_map<string, Class*>;

            auto *p = new unordered_map<string, Class*>{
                {"int", make_class("int", "<object>", {}, {})},
                {"boolean", make_class("boolean", "int", {}, {})},
                {"void", make_class("void", "<object>", {}, {})},
                {"String", make_class("String", "<object>", {}, {})},
                {"<object>", make_class("<object>", "<object>", {}, {})}
            };
            M = p;
            cerr << "new umap" << endl;
        }
        c = &(*M)[id];
        if (*id.rbegin() == ']' && !*c) {
            *c = make_class(id, string(id.begin(), id.end()-2), {}, {});
        }
    }

    Class& operator*() {
        assert(*c);
        return **c;
    }
    Class *operator->() {
        assert(*c);
        return *c;
    }
    // std::string id;
    Class **c;
    static std::unordered_map<std::string, Class*> *M;

private:
    Class *make_class(std::string id, std::string parent, std::vector<ParamDecl*> attrs, std::vector<Method*> methods);
};

struct Class: Node {
    Class(std::string id, std::string parent, std::vector<ParamDecl*> attrs, std::vector<Method*> methods)
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
    std::vector<ParamDecl*> attrs;
    std::vector<Method*> methods;
    void accept(Visitor *v) { v->visit(this); }

};


struct Method: Node {
    Method(std::string type, std::string id, std::vector<ParamDecl*> pl,
           std::vector<ParamDecl*> vl, std::vector<Statement*> sl, Expression *ret)
    : type(type), id(id), pl(pl), vl(vl), sl(sl), ret(ret) {}
    Type type;
    std::string id;
    std::vector<ParamDecl*> pl, vl;
    std::vector<Statement*> sl;
    Expression *ret;
    void accept(Visitor *v) { v->visit(this); }
};

// typedef std::pair<Type, std::string> ParamDecl;
struct ParamDecl {
    Type t;
    std::string id;
};
}

#endif
