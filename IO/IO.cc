#include <IO/IO.h>

#include <Math/Vector.h>

using namespace std;

namespace kepler {

ostream& operator<<(ostream& os, const DMatrix& v) {
    os << "{" << endl;
    for (const auto& x: v) {
        os << "[";
        for (const auto& y: x) {
            os << y << ", ";
        }
        os << "]" << endl;
    }
    os << "}" << endl;
    return os;
}

ostream& operator<<(ostream& os, const Vvector& v) {
    os << "{" << endl;
    for (const Vector& vec: v) {
        os << vec << endl;
    }
    os << "}" << endl;
    return os;
}

ostream& operator<<(ostream& os, const VMatrix& v) {
    os << "{" << endl;
    for (auto& vec: v) {
        os << vec << endl;
    }
    os << "}" << endl;
    return os;
}

ostream& operator<<(ostream& os, const set<int>& v) {
    os << "{";
    auto iter = begin(v);
    auto myend = end(v);
    while (iter != myend) {
        os << *iter;
        ++iter;
        if (iter != myend) {
            os << ", ";
        }
    }
    os << "}";
    return os;
}


vector<string> split(const string& str, const regex& delim) {
    vector<string> tokens;
    sregex_token_iterator it(str.begin(), str.end(), delim, -1);
    sregex_token_iterator reg_end;
    for (; it != reg_end; ++it) {
        tokens.push_back(it->str());
    }
    return tokens;
}

string trim(const string& s) {
    static const string whitespace = " \t\n";
    const auto strBegin = s.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) {
        return "";
    }
    const auto strEnd = s.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;
    return s.substr(strBegin, strRange);
}

}
