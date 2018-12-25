#include "program.hpp"


namespace ast{

std::string Type::remove_subscript(std::string s) const {
    return std::string(s.begin(), s.end()-2);
}

Type::operator bool() const {
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
            {"int", new Class("int", "<object>", {}, {})},
            {"boolean", new Class("boolean", "<object>", {}, {})},
            {"void", new Class("void", "<object>", {}, {})},
            {"String", new Class("String", "<object>", {}, {})},
            {"<object>", new Class("<object>", "<object>", {}, {})}
        };
        M = p;
    }
    auto p = M->insert({name, nullptr}).first;
    id = &p->first; c = &p->second;
    if (*id->rbegin() == ']' && !c) {
        *c = new Class(*id, remove_subscript(*id), {}, {}); // get rid of []
        // TODO: add dispatch, for length maybe.
    }

}

bool Type::operator==(const Type& rhs) const {
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
        // std::cerr << "<= ---------" << *now.id << std::endl;
        if (now == *this)  return true;
        now = (*now.c)->parent;
    }
    // std::cerr << " ======== "  << std::endl;
    return false;
}

}
