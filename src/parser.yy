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
    #include <vector>
    #include <parser_output.hpp>
    #include <ast.hpp>

    namespace yy {
        class scanner;
    }
}

%code {
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    #include <string>
    #include <vector>
    #include <ast/expression.hpp>
    #include <ast/statement.hpp>

    #include "scanner.hpp"
    using namespace ast;
    using std::move;
    using std::vector;


    #ifdef  yylex
    # undef yylex
    #endif
    #define yylex scanner.yylex
}

///////// Definiton of Tokens /////////
%token                  END     0   "end of file"

// expressions
%type <ast::Expression*> expr
%type <ast::Statement*> program
%type <std::vector<ast::Expression*> > non_empty_param_list param_list
%type <std::vector<ast::Statement*> > stmt_list
%type <ast::Statement*> stmt

// %token CLASS
// %token PUBLIC_STATIC_VOID_MAIN
// %token INT BOOLEAN VOID

// expression
%token <std::string> OBJECTID
%token <int> INT_CONST
%token <bool> BOOL_CONST
%token AND DOTLENGTH NEWINT NEW

// statement
%token IF ELSE WHILE PRINTLN
// %


///////// Precedence Declaration //////
%nonassoc '<'
%left AND
%left '+' '-'
%left '*' '/'
%right '!'
%left '.' DOTLENGTH
%precedence '[' ']'


%%

program
//   : expr { res.result = $$ = $1; }
  : stmt { res.result = $$ = $1; }
  ;

non_empty_param_list
  : expr[e] { $$ = vector<Expression*>{$e}; }
  | non_empty_param_list[el] ',' expr[e] { $$ = append($el, $e); }
  ;

param_list
  : { $$ = vector<Expression*>{}; }
  | non_empty_param_list[el] { $$ = $el; }
  ;

expr
  : expr[e1] AND expr[e2] { $$ = new Bop{Add, $e1, $e2}; }
  | expr[e1] '<' expr[e2] { $$ = new Bop{Le, $e1, $e2}; }
  | expr[e1] '+' expr[e2] { $$ = new Bop{Add, $e1, $e2}; }
  | expr[e1] '-' expr[e2] { $$ = new Bop{Minus, $e1, $e2}; }
  | expr[e1] '*' expr[e2] { $$ = new Bop{Mul, $e1, $e2}; }
  | expr[e1] '[' expr[e2] ']' { $$ = new Bop{Arr, $e1, $e2}; }
  | expr[e] DOTLENGTH {$$ = new Uop{Len, $e}; }
  | expr[e] '.' OBJECTID[o] '(' param_list[l] ')' { $$ = new Dispatch{$e, $o, $l}; }
  | INT_CONST[i] { $$ = new IntConst{$i}; }
  | BOOL_CONST[b] { $$ = new BoolConst{$b}; }
  | OBJECTID[o] { $$ = new Object{$o}; } // TODO: this
  | NEWINT '[' expr[e] ']' { $$ = new Uop{NewInt, $e}; }
  | NEW OBJECTID[o] "(" ")" { $$ = new NewObj{$o}; }
  | '{' expr[e] '}' { $$ = new Uop{Bracket, $e}; }
  | '!' expr[e] { $$ = new Uop{Not, $e}; }
  | '(' expr[e] ')' { $$ = new Uop{Bracket, $e}; }
  ;

stmt_list
  : { $$ = vector<Statement*>{}; }
  | stmt_list[sl] stmt[s] { $$ = append($sl, $s); }

stmt
  : '{' stmt_list[sl] '}'  { $$ = new Block{$sl}; }
  | IF '(' expr[e] ')' stmt[s1] ELSE stmt[s2] { $$ = new If{$e, $s1, $s2}; }
  | WHILE '(' expr[e] ')' stmt[s] { $$ = new While{$e, $s}; }
  | PRINTLN '(' expr[e] ')' ';' { $$ = new Println{$e}; }
//   | OBJECTID[o] '=' expr[e] ';' { $$ = new Assign{$o, $e}; }
//   | OBJECTID[o] '[' expr[e1] ']' '=' expr[e2] ';' { $$ = new ArrayAssign{$o, $e1, $e2}; }
  ;

%%

void yy::parser::error(const location_type &l, const std::string &m) {
    std::cerr << m << " at: " << l << std::endl;
    // throw yy::parser::syntax_error(l, m);
}