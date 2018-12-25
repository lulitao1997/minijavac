#include "utils.hpp"

int error_num;

static const char *cbegin = "\033[1;31m", *cend = "\033[0m";

std::ostream &complain(yy::location loc) {
    return std::cerr << loc << ", ";
}