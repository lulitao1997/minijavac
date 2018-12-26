#ifndef CODEGEN_HPP_
#define CODEGEN_HPP_

#include "ast.hpp"

namespace ast {

struct CodeGen: Visitor {
    void visit(Uop *e) {
    }
    void visit(Bop *e) {

    }


private:
    // helper functions.

    template<typename T, typename ...Tps>
    void O(T a, Tps ...args) {
        out << a << ' ';
        O(args...);
    }
    template<typename T>
    void O(T a) {
        out << a << std::endl;
    }
    void cmt(std::string a) {
        out << "# " << a << std::endl;
    }
    void labl(std::string a) {
        out << a << ":" << std::endl;
    }
    std::ostream& out = std::cout;
};

}

#endif