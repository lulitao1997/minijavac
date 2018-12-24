#include "utils.hpp"

int error_num;

std::ostream &complain(yy::location loc) {
    return std::cerr << loc << ", ";
}