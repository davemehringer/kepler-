/*
 * BodyFrameRotMatrix.h
 *
 *  Created on: Sep 8, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_BODYFRAMEROTMATRIX_H_
#define KEPLER_BODYFRAMEROTMATRIX_H_

#include <Math/Constants.h>

namespace kepler {

class RotMatrix3x3;

// Pure virtual base class providing access to the time dependent
// ecliptic to equatorial rotation matrix for an astronomical body
class BodyFrameRotMatrix {
public:
    virtual ~BodyFrameRotMatrix() {};

    // return the ecliptic to equatorial rotation matrix
    // for Julian Day t
    // vector_equatorial = vector_ecliptic*matrix_ecliptic_to_equatorial
    virtual const RotMatrix3x3& ecToEq(PrecType t, TimeUnit unit) const = 0;

    // return the equatorial to ecliptic rotation matrix
    // for Julian Day t
    // vector_ecliptic = vector_equatorial*matrix_equatorial_to_ecliptic
    virtual const RotMatrix3x3& eqToEc(PrecType t, TimeUnit unit) const = 0;

};

}

#endif
