#include "parser.hxx"
#include <scanner.hpp>
#include <ast/printer.hpp>
#include <ast/typechecker.hpp>
#include <parser_output.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    // string parser_input = "123.length + 456[2[0]] < 78 + !asdf.fun(1, 2, 3) * true\n";
    // string parser_input = "if (true) System.out.println(123); else if (false) {a=123;} else {}\n";

    cerr << yy::parser::token::OBJECTID << endl;

    ParserOutput out;
    // auto in = istringstream(parser_input);
    auto in = fstream("test.java", ios::in);
    yy::scanner s(&in);

    yy::parser p(s, out);
    auto *pr = new ast::Printer(cout);
    cerr << "Parse state: " << p.parse() << endl;
    // out.result->accept(pr);
    for (auto c: out.result) {
        c->accept(pr);
    }
    auto *tc = new ast::TypeChecker;
    for (auto c: out.result) {
        c->accept(tc);
    }
}