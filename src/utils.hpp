#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <location.hh>
#include <string>
#include <ostream>

extern int error_num;
std::ostream &complain(yy::location loc);

#endif