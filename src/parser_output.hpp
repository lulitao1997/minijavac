#ifndef PARSER_OUTPUT_HPP_
#define PARSER_OUTPUT_HPP_
#include <ast.hpp>
#include <vector>

struct ParserOutput {
    // ast::Expression *result;
    // ast::Statement *result;
    std::vector<ast::Class*> result;
};

#endif