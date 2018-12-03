%skeleton "lalr1.cc"
%require  "3.2"
%debug
%defines
%define api.namespace {yy}
%define parser_class_name {parser}


%define parse.error verbose
%define api.value.type variant
%define parse.assert

%locations // TODO: ???

%parse-param { yy::scanner &scanner } { ParserOutput &res }

%code requires{
    #include <stdexcept>
    #include <string>
    #include <parser_output.hpp>

    namespace yy {
        class scanner;
    }
}

%code {
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    #include <string>

    #include "scanner.hpp"

    #ifdef  yylex
    # undef yylex
    #endif
    #define yylex scanner.yylex
}

///////// Definiton of Tokens /////////
%token                  END     0   "end of file"

%token <int> INTEGER

%type <int> expr
%type <int> program


///////// Precedence Declaration //////
%left '+'

%%

program
  : expr { res.result = $$ = $1; }
  ;

expr
  : expr '+' expr { $$ = $1 + $3; }
  | INTEGER {
    //   std::cerr << "parser meet an int: " << $1 << std::endl;
      $$ = $1;
    }
  ;


%%

void yy::parser::error(const location_type &l, const std::string &m) {
    //cerr << "Parser Error" << err_msg << "at: " << l << endl;
    throw yy::parser::syntax_error(l, m);
}