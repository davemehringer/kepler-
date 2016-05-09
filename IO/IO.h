
#ifndef KEPLER_IO_H_
#define KEPLER_IO_H_

#include <Math/Types.h>

#include <ostream>
#include <regex>
#include <set>

using namespace std;

namespace kepler {

template <class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << "[";
    for (const auto& x: v) {
        os << x << ", ";
    }
    os << "]" << endl;
    return os;
}

ostream& operator<<(ostream& os, const DMatrix& v);

ostream& operator<<(ostream& os, const Vvector& v);

ostream& operator<<(ostream& os, const VMatrix& v);

ostream& operator<<(ostream& os, const set<int>& v);

vector<string> split(const string& str, const regex& delim);

string trim(const string& s);

}

#endif
