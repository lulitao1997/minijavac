%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {javac}
%define parser_class_name {Parser}

%code requires{
   namespace javac {
      class Driver;
      class Scanner;
   }
}

%parse-param { Scanner &scanner  }
%parse-param { Driver &driver  }

%location // TODO: ???

%define parse.trace
%define parse.error verbose

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   /* include for all driver functions */
   #include "javac_driver.hpp"

#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert


%%

%%

void javac::Parser::error(const location_type &l, const std::string &err_msg) {
    cerr << "Parser Error" << err_msg << "at: " << l << endl;
}