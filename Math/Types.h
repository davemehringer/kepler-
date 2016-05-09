/*
 * Types.h
 *
 *  Created on: Sep 19, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_TYPES_H_
#define KEPLER_TYPES_H_

#include <vector>

using namespace std;

namespace kepler {

class Vector;

using PrecType = long double;

using DMatrix = vector<vector<PrecType> >;

using Vvector = vector<Vector>;

using VMatrix = vector<Vvector>;

}

#endif
