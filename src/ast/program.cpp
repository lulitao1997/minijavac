#include "program.hpp"
#include <unordered_set>

using namespace ast;
using namespace std;

Class *Class::get_parent() {
    return *parent.c;
}

void Class::inherit() {
    if (inherited || !parent) return;
    Class *pa = get_parent();
    if (pa != this) {
        pa->inherit();
        for (auto a: pa->attrs) attrs.push_back(a);
        for (auto m: pa->methods) methods.push_back(m);
    }
    inherited = true;
}

