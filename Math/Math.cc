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
