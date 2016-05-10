//# IO.h
//# Copyright (C) 2016 David Mehringer
//# 
//#     This program is free software: you can redistribute it and/or modify
//#     it under the terms of the GNU General Public License as published by
//#     the Free Software Foundation, either version 3 of the License, or
//#     (at your option) any later version.
//# 
//#     This program is distributed in the hope that it will be useful,
//#     but WITHOUT ANY WARRANTY; without even the implied warranty of
//#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#     GNU General Public License for more details.
//# 
//#     You should have received a copy of the GNU General Public License
//#     along with this program.  If not, see <http://www.gnu.org/licenses/>.
//# 

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
