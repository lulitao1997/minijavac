#include "program.hpp"


namespace ast{

std::unordered_map<std::string, Class*> *Type::M = nullptr;

Class *Type::make_class(std::string id, std::string parent, std::vector<ParamDecl*> attrs, std::vector<Method*> methods) {
    return new Class(id, parent, attrs, methods);
}

}
