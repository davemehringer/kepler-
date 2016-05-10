//# Math.cc
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
/*
 * Math.cc
 *
 *  Created on: Sep 5, 2015
 *      Author: dmehring
 */

#include "Math.h"

#include <algorithm>
#include <cmath>

using namespace std;

namespace kepler {

bool near(PrecType x, PrecType y, PrecType epsilon) {
	if (x == y) {
		return true;
	}
	if (x == 0 || y == 0 || (abs(x) < epsilon && abs(y) < epsilon)) {
		return abs(x - y) <= epsilon;
	}
	auto mymin = min(abs(x), abs(y));
	return abs((x -y)/mymin) <= epsilon;
}

bool nearAbs(PrecType x, PrecType y, PrecType epsilon) {
    return abs(x - y) <= epsilon;
}


}
