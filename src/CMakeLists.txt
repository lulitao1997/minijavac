find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(yy_parser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cxx
             COMPILE_FLAGS --defines=${CMAKE_CURRENT_BINARY_DIR}/parser.hxx
             VERBOSE REPORT_FILE)
FLEX_TARGET(yy_scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cxx
            COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/scanner.hxx)
ADD_FLEX_BISON_DEPENDENCY(yy_scanner yy_parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(deps
    utils.cpp
    ast/type.cpp
    ast/program.cpp
    ${FLEX_yy_scanner_OUTPUTS}
    ${BISON_yy_parser_OUTPUTS}
)

add_executable(parser_exe
    parser_exe.cpp
    ${deps}
)

add_executable(lexer_exe
    lexer_exe.cpp
    ${deps}
)

set_property(TARGET parser_exe PROPERTY CXX_STANDARD 14)
# enable_testing()
# add_test(NAME basic
#          COMMAND ${CMAKE_SOURCE_DIR}/test/test0.pl
#          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
