#include "program.hpp"


namespace ast{

Class *Type::make_class(std::string id, std::string parent, std::vector<Var*> attrs, std::vector<Method*> methods) const {
    return new Class(id, parent, attrs, methods);
}
std::string Type::remove_subscript(std::string s) const {
    return std::string(s.begin(), s.end()-2);
}

Type::operator bool() const {
    std::cerr << "check type:::" << *id << ": " << *c << std::endl;
    if (!is_array())
        return *c;
    return bool(Type(remove_subscript(*id)));
}
Type Type::array_body() const {
    return Type(remove_subscript((*id)));
}
bool Type::is_array() const {
    return *((*id).rbegin()) == ']';
}
const std::vector<Method*>& Type::methods() {
    return (*c)->methods;
}

std::unordered_map<std::string, Class*> *Type::M;

Type::Type(std::string name) {
    using namespace std;
    if (!M) {
        M = new unordered_map<string, Class*>;

        auto *p = new(M) unordered_map<string, Class*>{
            {"int", make_class("int", "<object>", {}, {})},
            {"boolean", make_class("boolean", "<object>", {}, {})},
            {"void", make_class("void", "<object>", {}, {})},
            {"String", make_class("String", "<object>", {}, {})},
            {"<object>", make_class("<object>", "<object>", {}, {})}
        };
        M = p;
        // cerr << "new umap" << endl;
    }
    c = &(*M)[name];
    // auto p = M->insert({name, nullptr}).first;
    // id = &p->first; // c = &p->second;
    id = new string(name);
    // assert(c == &(*M)[name]);
    if (*id->rbegin() == ']' && !c) {
        *c = make_class(*id, remove_subscript(*id), {}, {}); // get rid of []
        // TODO: add dispatch, for length maybe.
    }

}

bool Type::operator==(const Type& rhs) const {
    std:: cerr << "type === " << *id << ':' << *c << ", " << *rhs.id << ":" <<  *rhs.c << std::endl;
    return *c == *rhs.c;
}
bool Type::operator!=(const Type& rhs) const {
    return *c != *rhs.c;
}

bool Type::is_arithmetic() const {
    return *this == Type("int") || *this == Type("boolean");

}
bool Type::is_compatible(const Type& rhs) const {
    if (this->is_arithmetic()) {
        return rhs.is_arithmetic();
    }

    /// Base = Derived; ok
    Type now = rhs;
    while (now && *now.id != "<object>") {
        std::cerr << "<= ---------" << *now.id << std::endl;
        if (now == *this)  return true;
        now = (*now.c)->parent;
    }
    std::cerr << " ======== "  << std::endl;
    return false;
}

}
