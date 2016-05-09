/*
 * Math.h
 *
 *  Created on: Sep 5, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_MATH_H_
#define KEPLER_MATH_H_

#include <Math/Types.h>

namespace kepler {

// are x and y relatively close to each other?
bool near(PrecType x, PrecType y, PrecType epsilon);

// are x and y absolutely close to each other?
bool nearAbs(PrecType x, PrecType y, PrecType epsilon);

}

#endif
