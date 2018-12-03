#include "parser.hxx"
#include <scanner.hpp>
#include <parser_output.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    string parser_input = "123 + 567 + 7\n";

    ParserOutput out;
    auto in = istringstream(parser_input);
    yy::scanner s(&in);
    // while (s.yylex()) ;
    int tktype = 0;
    yy::parser::location_type yylloc;
    while (tktype = s.yylex(new yy::parser::semantic_type, &yylloc)) {
        cout << tktype << ", " << yylloc << endl;
        if (!tktype) break;
    }
    // yy::parser p(s, out);
    // int result = p.parse();
    std::cout << out.result << std::endl;
}