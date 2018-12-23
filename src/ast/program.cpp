#include "program.hpp"


namespace ast{

Class *Type::make_class(std::string id, std::string parent, std::vector<ParamDecl*> attrs, std::vector<Method*> methods) {
    return new Class(id, parent, attrs, methods);
}

std::unordered_map<std::string, Class*> *Type::M;

Type::Type(std::string id) {
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
        // cerr << "new umap" << endl;
    }
    c = &(*M)[id];
    if (*id.rbegin() == ']' && !*c) {
        *c = make_class(id, string(id.begin(), id.end()-2), {}, {});
    }

}

}
