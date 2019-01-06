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
    #include <vector>
    #include <ast/expression.hpp>
    #include <ast/statement.hpp>
    #include <ast/program.hpp>

    #include "scanner.hpp"
    using namespace ast;
    using std::vector;
    using std::string;


    #include "location.hh"
    namespace yy {
      location cur_loc;
    }

    // #define YYLLOC_DEFAULT(Cur, Rhs, N) cur_loc = (Cur);
    #define YYLLOC_DEFAULT(Cur, Rhs, N) \
    do { \
      if (N) {\
        (Cur).begin = YYRHSLOC(Rhs, 1).begin; \
        (Cur).end = YYRHSLOC(Rhs, N).end; \
      } \
      else \
        (Cur) = YYRHSLOC(Rhs, 0); \
      cur_loc = (Cur); \
    } while(0)

    #ifdef  yylex
    # undef yylex
    #endif
    #define yylex scanner.yylex
}

///////// Definiton of Tokens /////////
%token                  ERROR
// %token                  END     0   "end of file"

// expressions
%type <ast::Expression*> expr
// %type <ast::Statement*> program
%type <ast::Program*> program
%type <std::vector<ast::Expression*>> non_empty_param_list param_list
%type <std::vector<ast::Statement*>> stmt_list // non_empty_stmt_list
%type <ast::Statement*> stmt

%token <std::string> OBJECTID
%token <int> INT_CONST
%token <bool> BOOL_CONST
%token AND LENGTH NEW NEWARR // NEWARR = "[]"

// statement
%token IF ELSE WHILE PRINTLN

// class
%token CLASS EXTENDS PUBLIC RETURN PUBLIC_STATIC VOID MAIN
%token INT

%type <std::vector<ast::Class*>> class_decl_list class_list_with_main
%type <ast::Class*> main_class class_decl
%type <std::string> type_
%type <ast::Param*> param_decl
%type <ast::Var*> var_decl
%type <std::vector<ast::Var*>> attr_list
%type <std::vector<ast::Param*>> non_empty_param_decl_list param_decl_list
%type <ast::Method*> method_decl main_method
%type <std::vector<ast::Method*>> method_decl_list

///////// Precedence Declaration ////////
%precedence ')'
%precedence ELSE

%right '='

%left '!'

%nonassoc '<'

%left AND
%left '+' '-'
%left '*' '/'
%left '.'
%precedence '['

%%

program
  : class_list_with_main[cl] { res.result = $$ = new Program($cl); }
  ;

class_list_with_main
  : main_class[m] { $$ = single($m); }
  | main_class[m] class_decl_list[cl] { $$ = append(single($m), $cl); }
  | class_decl_list[cl] main_class[m] { $$ = append($cl, $m); }
  | class_decl_list[c1] main_class[m] class_decl_list[c2]
    { $$ = append(append($c1, $m), $c2); }
  ;

non_empty_param_list
  : expr[e] { $$ = single($e); }
  | non_empty_param_list[el] ',' expr[e] { $$ = append($el, $e); }
  ;

param_list
  : %empty { $$ = {}; }
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
  | expr[e] '.' OBJECTID[o] '(' param_list[l] ')' { cur_loc = @$ = @o; $$ = new Dispatch{$e, $o, $l}; }
  | OBJECTID[o] '(' param_list[l] ')' { $$ = new Dispatch{new Object{"this"}, $o, $l}; } // EXT: optional `this`
  | INT_CONST[i] { $$ = new IntConst{$i}; }
  | BOOL_CONST[b] { $$ = new BoolConst{$b}; }
  | OBJECTID[o] { $$ = new Object{$o}; } // TODO: this
  // | NEWINT '[' expr[e] ']' { $$ = new Uop{NewInt, $e}; }
  | NEW type_[t] '[' expr[e] ']' { $$ = new NewArr{Type($t+"[]"), $e}; }
  | NEW type_[t] '(' ')' { $$ = new NewObj{Type($t)}; }
  | '{' expr[e] '}' { $$ = new Uop{Bracket, $e}; }
  | '!' expr[e] { $$ = new Uop{Not, $e}; }
  | '(' expr[e] ')' { $$ = new Uop{Bracket, $e}; }
  ;

// non_empty_stmt_list

stmt_list
  // : stmt[s] { $$ = single($s); }
  : %empty { $$ = {}; }
  | stmt_list[sl] stmt[s] { $$ = append($sl, $s); }

stmt
  : '{' stmt_list[sl] '}'  { $$ = new Block{$sl}; }
  | IF '(' expr[e] ')' stmt[s1] ELSE stmt[s2] { $$ = new If{$e, $s1, $s2}; }
  | IF '(' expr[e] ')' stmt[s1] { $$ = new If{$e, $s1, nullptr}; }
  | WHILE '(' expr[e] ')' stmt[s] { $$ = new While{$e, $s}; }
  | PRINTLN '(' expr[e] ')' ';' { $$ = new Println{$e}; }
  | OBJECTID[o] '=' expr[e] ';' { $$ = new Assign{new Object{$o}, $e}; }
  | OBJECTID[o] '[' expr[e1] ']' '=' expr[e2] ';' { $$ = new ArrAssign{new Object{$o}, $e1, $e2}; }
  | var_decl[v] { $$ = $v; }
  | RETURN expr[e] ';' { $$ = new Return{$e}; }
  ;

type_
  : OBJECTID[t] { $$ = $t; }
  // | type_[t] '[' ']' { $$ = $t + "[]"; }
  | type_[t] NEWARR { $$ = $t + "[]"; }

var_decl
  : type_[t] OBJECTID[o] ';' { $$ = new Var{Type($t), $o}; }

attr_list
  : %empty { $$ = {}; }
  | attr_list[vl] var_decl[v] { $$ = append($vl, $v); }

param_decl
  : type_[t] OBJECTID[o] { $$ = new Param{Type($t), $o}; }

non_empty_param_decl_list
  : param_decl[p] { $$ = single($p); }
  | non_empty_param_decl_list[pl] ',' param_decl[p] { $$ = append($pl, $p); }

param_decl_list
  : %empty { $$ = {}; }
  | non_empty_param_decl_list[pl] { $$ = $pl; }

method_decl
  : PUBLIC type_[t] OBJECTID[o] '(' param_decl_list[pl] ')' '{'
      // var_decl_list[vl]
      stmt_list[sl]
    '}'
    { $$ = new Method($t, $o, $pl, $sl); }
  | PUBLIC type_[t] OBJECTID[o] '(' param_decl_list[pl] ')' '{'
      // var_decl_list[vl]
      error
      stmt_list[sl]
    '}'
    { $$ = new Method($t, $o, $pl, $sl); }

method_decl_list
  : %empty { $$ = {}; }
  | method_decl_list[ml] method_decl[m] { $$ = append($ml, $m); }

class_decl
  : CLASS OBJECTID[o] EXTENDS OBJECTID[oe] '{'
      attr_list[vl]
      method_decl_list[ml]
    '}'
    { cur_loc = @$ = @o;
      $$ = new Class($o, $oe, $vl, $ml); }
  | CLASS OBJECTID[o] '{'
      attr_list[vl]
      method_decl_list[ml]
    '}'
    { cur_loc = @$ = @o;
      $$ = new Class($o, "<object>", $vl, $ml); }
  | CLASS error
      attr_list[vl]
      method_decl_list[ml]
    '}'
    { $$ = new Class("<syntax_error>", "<object>", $vl, $ml); }
  // { $$ = nullptr; }

class_decl_list
  : class_decl[c] { $$ = single($c); }
  | class_decl_list[cl] class_decl[c] { $$ = append($cl, $c); }

main_method
  : PUBLIC_STATIC VOID MAIN '(' param_decl_list[pl] ')' '{'
    stmt_list[sl]
  '}'
  { $$ = new Method("void", "main",
                    $pl, $sl); }
main_class
  : CLASS OBJECTID[o] '{'
      // attr_list[vl]
      main_method[m]
    '}'
    { $$ = new Class($o, "<object>", {}, single($m)); }

%%

void yy::parser::error(const location_type &l, const std::string &m) {
    complain(l) << m << std::endl;
    // std::cerr << m << " at: " << l << std::endl;
    // throw yy::parser::syntax_error(l, m);
}
