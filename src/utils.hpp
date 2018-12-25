#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <location.hh>
#include <string>
#include <vector>

extern int error_num;
extern std::vector<std::string> lines;
std::ostream &complain(yy::location loc);
void output_error();

#endif