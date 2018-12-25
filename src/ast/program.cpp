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

    for (auto i: methods) for (auto j: methods) {
        if (i != j && *i == *j) {
            complain(i->loc) << "dulplicate method definion `" << i->id << "` with " << j->loc << endl;
        }
    }

    if (pa != this) {
        pa->inherit();
        // for (auto a: pa->attrs) attrs.push_back(a);
        attrs = append(pa->attrs, attrs);
        vector<Method*> t = pa->methods, t2;
        for (auto m: methods) {
            bool found = false;
            for (auto &j: t) if (j == m) {
                found = true;
                j = m;
                break; // override parent method
            }
            if (!found) t2.push_back(m);
        }
        methods = append(t, t2);
    }
    inherited = true;
}

