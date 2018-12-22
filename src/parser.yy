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
    #include <utility>
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
    #include <utility>
    #include <string>
    #include <vector>
    #include <ast/expression.hpp>
    #include <ast/statement.hpp>
    #include <ast/program.hpp>

    #include "scanner.hpp"
    using namespace ast;
    using std::move;
    using std::vector;
    using std::string;


    #ifdef  yylex
    # undef yylex
    #endif
    #define yylex scanner.yylex
}

///////// Definiton of Tokens /////////
%token                  END     0   "end of file"

// expressions
%type <ast::Expression*> expr
// %type <ast::Statement*> program
%type<std::vector<ast::Class*>> program
%type <std::vector<ast::Expression*>> non_empty_param_list param_list
%type <std::vector<ast::Statement*>> stmt_list
%type <ast::Statement*> stmt

%token <std::string> OBJECTID
%token <int> INT_CONST
%token <bool> BOOL_CONST
%token AND LENGTH NEWINT NEW

// statement
%token IF ELSE WHILE PRINTLN

// class
%token CLASS EXTENDS PUBLIC RETURN STATIC VOID MAIN
%type <std::vector<ast::Class*>> class_decl_list
%type <ast::Class*> main_class class_decl
%type <std::string> type_
%type <ast::ParamDecl> param_decl var_decl
%type <std::vector<ast::ParamDecl>> var_decl_list non_empty_param_decl_list param_decl_list
%type <ast::Method*> method_decl main_method
%type <std::vector<ast::Method*>> method_decl_list

///////// Precedence Declaration ////////
// %precedence IF
// $precedence ELSE

%right '='
%left '!'

%nonassoc '<'

%left AND
%left '+' '-'
%left '*' '/'
%left '.' '['

%%

program
//   : expr { res.result = $$ = $1; }
//   : stmt { res.result = $$ = $1; }
  : main_class[m] class_decl_list[cl] { res.result = $$ = append($cl, $m); }
  ;

non_empty_param_list
  : expr[e] { $$ = vector<Expression*>{$e}; }
  | non_empty_param_list[el] ',' expr[e] { $$ = append($el, $e); }
  ;

param_list
  : %empty { $$ = vector<Expression*>{}; }
  | non_empty_param_list[el] { $$ = $el; }
  ;

expr
  : expr[e1] AND expr[e2] { $$ = new Bop{Add, $e1, $e2}; }
  | expr[e1] '<' expr[e2] { $$ = new Bop{Le, $e1, $e2}; }
  | expr[e1] '+' expr[e2] { $$ = new Bop{Add, $e1, $e2}; }
  | expr[e1] '-' expr[e2] { $$ = new Bop{Minus, $e1, $e2}; }
  | expr[e1] '*' expr[e2] { $$ = new Bop{Mul, $e1, $e2}; }
  | expr[e1] '[' expr[e2] ']' { $$ = new Bop{Arr, $e1, $e2}; }
  | expr[e] '.' LENGTH {$$ = new Uop{Len, $e}; }
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
  : %empty { $$ = vector<Statement*>{}; }
  | stmt_list[sl] stmt[s] { $$ = append($sl, $s); }

stmt
  : '{' stmt_list[sl] '}'  { $$ = new Block{$sl}; }
  | IF '(' expr[e] ')' stmt[s1] ELSE stmt[s2] { $$ = new If{$e, $s1, $s2}; }
  | WHILE '(' expr[e] ')' stmt[s] { $$ = new While{$e, $s}; }
  | PRINTLN '(' expr[e] ')' ';' { $$ = new Println{$e}; }
  | OBJECTID[o] '=' expr[e] ';' { $$ = new Assign{$o, $e}; }
  | OBJECTID[o] '[' expr[e1] ']' '=' expr[e2] ';' { $$ = new ArrAssign{$o, $e1, $e2}; }
  ;

type_
  : OBJECTID[t] { $$ = $t; }
  | OBJECTID[t] '[' ']' { $$ = $t + string("[]"); }

var_decl
  : type_[t] OBJECTID[o] ';' { $$ = make_pair($t, $o); }

var_decl_list
  : %empty { $$ = vector<ParamDecl>{}; }
  | var_decl_list[vl] var_decl[v] { $$ = append($vl, $v); }

param_decl
  : type_[t] OBJECTID[o] { $$ = make_pair($t, $o); }

non_empty_param_decl_list
  : param_decl[p] { $$ = vector<ParamDecl>{$p}; }
  | non_empty_param_decl_list[pl] ',' param_decl[p] { $$ = append($pl, $p); }

param_decl_list
  : %empty { $$ = vector<ParamDecl>{}; }
  | non_empty_param_decl_list[pl] { $$ = $pl; }

method_decl
  : PUBLIC type_[t] OBJECTID[o] '(' param_decl_list[pl] ')' '{'
      var_decl_list[vl]
      stmt_list[sl]
      RETURN expr[e] ';'
    '}'
    { $$ = new Method($t, $o, $pl, $vl, $sl, $e); }

method_decl_list
  : %empty { $$ = vector<Method*>{}; }
  | method_decl_list[ml] method_decl[m] { $$ = append($ml, $m); }

class_decl
  : CLASS OBJECTID[o] EXTENDS OBJECTID[oe] '{'
      var_decl_list[vl]
      method_decl_list[ml]
    '}'
    { $$ = new Class($o, $oe, $vl, $ml); }
  | CLASS OBJECTID[o] '{'
      var_decl_list[vl]
      method_decl_list[ml]
    '}'
    { $$ = new Class($o, "", $vl, $ml); }

class_decl_list
  : %empty { $$ = vector<Class*>{}; }
  | class_decl_list[cl] class_decl[c] { $$ = append($cl, $c); }

main_method
  : PUBLIC STATIC VOID MAIN '(' param_decl_list[pl] ')' '{'
    stmt[s]
  '}'
  { $$ = new Method(string("void"), string("main"),
                    $pl, vector<ParamDecl>{},
                    vector<Statement*>{}, nullptr); }
main_class
  : CLASS OBJECTID[o] '{'
      main_method[m]
    '}'
    { $$ = new Class(std::string("main"), std::string(""), vector<ParamDecl>{}, single($m)); }

%%

void yy::parser::error(const location_type &l, const std::string &m) {
    std::cerr << m << " at: " << l << std::endl;
    // throw yy::parser::syntax_error(l, m);
}