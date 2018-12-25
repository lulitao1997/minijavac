#include "parser.hxx"
#include <scanner.hpp>
#include <ast/printer.hpp>
#include <ast/typechecker.hpp>
#include <parser_output.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc > 2) {
        cerr << "Usage: " << argv[0] << " file.java" << endl;
        return -1;
    }

    cout << string(5, '^') << endl;

    ParserOutput out;
    auto *in = (argc == 1 ? &cin : new fstream(argv[1]));
    yy::scanner s(in);

    yy::parser parser(s, out);
    int parse_ret = parser.parse();
    if (parse_ret) cerr << "parse failed: " << parse_ret << endl;
    else {
        auto *printer = new ast::Printer(cout);
        ast::Program *program = out.result;
        // program->accept(printer);
        auto *tchecker = new ast::TypeChecker;
        program->accept(tchecker);
    }
    output_error();
}