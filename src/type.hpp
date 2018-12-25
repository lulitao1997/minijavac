#ifndef TYPE_HPP_
#define TYPE_HPP_

#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>

namespace ast {

struct Class;
struct Method;

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
    bool is_array() const ;
    Type array_body() const ;
    const std::vector<Method*>& methods();
    friend class Class;
protected:
    Class **c;
    const std::string *id;
    static std::unordered_map<std::string, Class*> *M;
    std::string remove_subscript(std::string s) const;
};

}

#endif