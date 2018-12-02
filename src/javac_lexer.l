%{

#include <string>
#include "javac_lexer.hpp"

#define YY_DECL int javac::Scanner::yylex(javac::Parser::semantic_type *const lval, javac::MC_Parser::location_type *loc)
using token = javac::Parser::Token;

// #define yyterminate() return( token::END )

/* update location when matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option debug
%option nodefault
%option yyclass="javac::Scanner"
%option noyywrap
%option c++

%%

%%