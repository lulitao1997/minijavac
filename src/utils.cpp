#include "utils.hpp"
#include <sstream>
using namespace std;

int error_num;
vector<string> lines;

static const char *reds = "\033[1;31m", *rede = "\033[0m";
static vector<ostringstream> msgs;
static vector<yy::location> locs;

std::ostream &complain(yy::location loc) {
    // return std::cerr << loc << ", ";
    error_num++;
    locs.push_back(loc);
    msgs.emplace_back();
    return *msgs.rbegin();
    // cerr << reds << "ERROR: " << rede << loc << ": " << endl << lines[loc.begin.line] << reds << string(' ', start) << rede << endl;
}

void output_error() {
    int idx = 0;
    for (const auto &s: locs) {
        int start = s.begin.column, end = (s.begin.line == s.end.line ? s.end.column : lines[s.begin.line].size());
        cerr << s << reds << " ERROR: "  << rede << msgs[idx++].str();
        if (!start || !end || (end-start) < 1) continue;
        cerr
             << lines[s.begin.line-1]
             << reds << string(start-1, ' ') << string(end-start, '^') << rede << endl;
    }
}