#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_
#include "ast.hpp"
#include <string>
#include <vector>

namespace ast {

struct Var;
struct Method;


struct Class: Node {
    Class(std::string id, std::string parent, std::vector<ParamDecl> attrs, std::vector<Method*> methods)
    : id(id), parent(parent), attrs(attrs), methods(methods)
    {}

    std::string id, parent;
    // std::vector<Var*> attrs;
    std::vector<ParamDecl> attrs;
    std::vector<Method*> methods;
    void accept(Visitor *v) { v->visit(this); }
};

struct Method: Node {
    Method(std::string type, std::string id, std::vector<ParamDecl> pl,
           std::vector<ParamDecl> vl, std::vector<Statement*> sl, Expression *ret)
    : type(type), id(id), pl(pl), vl(vl), sl(sl), ret(ret) {}
    std::string type, id;
    std::vector<ParamDecl> pl, vl;
    std::vector<Statement*> sl;
    Expression *ret;
    void accept(Visitor *v) { v->visit(this); }
};

}

#endif