#ifndef SCANNER_HPP_
#define SCANNER_HPP_

#undef yyFlexLexer //TODO: Why???
#include <FlexLexer.h>
#include "parser.hxx"

#ifdef  YY_DECL
# undef YY_DECL
#endif
#define YY_DECL                                 \
    int yy::scanner::yylex(                     \
            yy::parser::semantic_type* yylval,  \
            yy::parser::location_type* yylloc)

namespace yy {

class scanner : public yyFlexLexer {
public:
    explicit scanner(std::istream* in=0, std::ostream* out=0);
    int yylex(parser::semantic_type* yylval,
              parser::location_type* yylloc);
};

}

#endif