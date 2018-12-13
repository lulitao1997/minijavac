#include "parser.hxx"
#include <scanner.hpp>
#include <ast/printer.hpp>
#include <parser_output.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    // string parser_input = "123.length + 456[2[0]] < 78 + !asdf.fun(1, 2, 3) * true\n";
    string parser_input = "if (true) System.out.println(123); else {}";

    ParserOutput out;
    auto in = istringstream(parser_input);
    yy::scanner s(&in);

    yy::parser p(s, out);
    auto *pr = new ast::Printer(cerr);
    cerr << "Parse state: " << p.parse() << endl;
    out.result->accept(pr);
}