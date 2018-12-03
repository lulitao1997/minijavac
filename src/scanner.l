%option debug
%option nodefault
%option yyclass="yy::scanner"
%option noyywrap
%option c++

%{

#include <string>
#include "scanner.hpp"

// because we use split symbols, token type is int.
using tk = yy::parser::token; // less verbose

// #define yyterminate() return( token::END )

/* update location when matching */
#define YY_USER_ACTION yylloc->columns(yyleng); // TODO: check col and line number...
%}

VAR_CHARSET [a-zA-Z0-9_]
WHITESPACE [ \f\r\t\v]

%%

%{
    yylloc->step();
%}

{WHITESPACE}+ {}

\n { yylloc->lines(); }

[0-9]+ {
    // std::cerr << "lexer meet an int: " << atoi(yytext) << std::endl;
    yylval->emplace<int>(atoi(yytext));
    return tk::INTEGER;
}

. {
    // std::cerr << "fuck fuck" << yytext << std::endl;
    return yytext[0];
}

%%

yy::scanner::scanner(std::istream *in, std::ostream *out)
        : yyFlexLexer(in, out)
{}