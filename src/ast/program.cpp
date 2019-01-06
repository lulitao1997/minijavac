#include "program.hpp"
#include "expression.hpp"
#include <unordered_set>

using namespace std;

namespace ast {

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

    if (pa && pa != this) {
        pa->inherit();
        // for (auto a: pa->attrs) attrs.push_back(a);
        attrs = append(pa->attrs, attrs);
        vector<Method*> t = pa->methods, t2;
        for (auto m: methods) {
            bool found = false;
            for (auto &j: t) {
                // cerr << " son cmp: " << m->id << ", " << j->id << endl;
                if (*j == *m) {
                    // cerr << "overload !!!" << endl;
                    found = true;
                    j = m;
                    break; // override parent method
                }
            }
            if (!found) {
                t2.push_back(m);
                // cerr << "new method !!!" << pa->id << "::" << id << "." << m->id << ", " << pa->methods.size()<< endl;
            }
        }
        methods.clear();
        methods = append(t, t2);
    }
    inherited = true;
    // cerr << "finished: " << id << ",  " << methods.size() << endl;
}

bool Method::operator==(Method& rhs) const {
    // std::cerr << "cmp:::" << id << ", " << rhs.id << std::endl;
    if (id != rhs.id || pl.size() != rhs.pl.size())
        return false;
    // return true;
    int idx = 0;
    for (Param *p: pl) {

        // std::cerr << "typ: " << p->t << ", " << rhs.pl[idx]->t << std::endl;
        if (p->type != rhs.pl[idx]->type) {
            return false;
        }
        idx++;
    }
    // std::cerr << "same cmp:::" << id << ", " << rhs.id << std::endl;
    return true;
}

bool Method::is_compatible(Dispatch *o) const {
    int idx=0;
    if (o->id != id || pl.size() != o->param_list.size())
        return false;
    for (auto t: pl) {
        if (!t->type.is_compatible(o->param_list[idx++]->type))
            return false;
    }
    // return type.is_compatible(o->type);
    return true; // cannot overload by return type
}

template<typename V>
void out_type(const V& v, ostream&out, const char *sep=", ") {
    bool flag = false;
    for (auto s: v) {
        if (flag) out << sep;
        else flag = true;
        out << s->type;
    }
}

std::ostream& operator<<(std::ostream& out, Method& rhs) {
    out << rhs.type << ' ' << rhs.id << '(';
    out_type(rhs.pl, out);
    return out << ')';
}

std::ostream& operator<<(std::ostream& out, Dispatch& rhs) {
    out << rhs.id << '(';
    out_type(rhs.param_list, out);
    out << ')';
    return out;
}

}
