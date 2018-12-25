#include <scanner.hpp>
#include <parser_output.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {

    ParserOutput out;
    auto in = fstream("test.java", ios::in);
    yy::scanner s(&in);
    // while (s.yylex()) ;

    int tktype = 0;
    yy::parser::location_type yylloc;
    while ((tktype = s.yylex(new yy::parser::semantic_type, &yylloc))) {
        cout << tktype << ", " << yylloc << endl;
        if (!tktype) break;
    }
    cerr << "end: " << tktype << endl;

    for (auto s: lines) cout << s << endl;
}