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
static std::string cur_line;

/* update location when matching */
#define YY_USER_ACTION \
    yylloc->step(); \
    yylloc->columns(yyleng); \
    cur_line += yytext; \

// TODO: check col and line number...
static void ins_newline() {
    lines.push_back(cur_line);
    cur_line.clear();
}

%}

VAR_CHARSET [a-zA-Z0-9_]
WHITESPACE [ \f\r\t\v]

 /*
%x STR_QUOTE
%x STR_ERROR
 */
%x BLK_CMT
%x LINE_CMT

%%

{WHITESPACE}+ {}

\n {
    ins_newline();
    yylloc->lines();
}

"/*" { BEGIN(BLK_CMT); }
"*/" {
    complain(cur_loc) << "unmatched *\\\n";
    BEGIN(INITIAL);
    return tk::ERROR;
}
<BLK_CMT>"/*" {}
<BLK_CMT>"*/" { BEGIN(INITIAL); }
<BLK_CMT>"\n" {
    ins_newline();
    yylloc->lines();
}
<BLK_CMT>. {}

"//" { BEGIN(LINE_CMT); }
<LINE_CMT>. {}
<LINE_CMT>"\n" {
    ins_newline();
    BEGIN(INITIAL);
    yylloc->lines();
}

[0-9]+ {
    yylval->emplace<int>(atoi(yytext));
    return tk::INT_CONST;
}

"true" {
    yylval->emplace<bool>(true);
    return tk::BOOL_CONST;
}

"false" {
    yylval->emplace<bool>(false);
    return tk::BOOL_CONST;
}

"&&" { return tk::AND; }
 /*
"new int" { return tk::NEWINT; }
 */
"new" { return tk::NEW; }
"["{WHITESPACE}*"]" { return tk::NEWARR; }
"length" { return tk::LENGTH; }


"if" { return tk::IF; }
"else" { return tk::ELSE; }
"while" { return tk::WHILE; }
"System.out.println" { return tk::PRINTLN; }

"class" { return tk::CLASS; }
"extends" { return tk::EXTENDS; }
"public" { return tk::PUBLIC; }
"public"{WHITESPACE}+"static" { return tk::PUBLIC_STATIC; }
"return" { return tk::RETURN; }
"void" { return tk::VOID; }
"main" { return tk::MAIN; }

 /*
"int" { returnt tk::INT; }
"String" { return tk::M_STRING; }
 */


[_a-zA-Z]{VAR_CHARSET}* {
    yylval->emplace<std::string>(yytext);
    return tk::OBJECTID;
}

. {
    return yytext[0];
}

%%

yy::scanner::scanner(std::istream *in, std::ostream *out)
        : yyFlexLexer(in, out)
{}

// void report_error(const std::string& error_msg) {
//     yylval->emplace<std::string>(error_msg);
// }