/*
 * BodyFrameRotMatrix.h
 *
 *  Created on: Sep 8, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_NeptuneFRAMEROTMATRIX_H_
#define KEPLER_NeptuneFRAMEROTMATRIX_H_

#include <BodyFrameRotMatrix/BodyFrameRotMatrix.h>
#include <Math/RotMatrix3x3.h>

#include <map>

using namespace std;

namespace kepler {

class NeptuneFrameRotMatrix: public BodyFrameRotMatrix {
public:

    NeptuneFrameRotMatrix();

    ~NeptuneFrameRotMatrix();

    // return the ecliptic to equatorial rotation matrix
    // for Julian Day t
    // vector_equatorial = vector_ecliptic*matrix_ecliptic_to_equatorial
    const RotMatrix3x3& ecToEq(PrecType t, TimeUnit unit) const ;

    // return the equatorial to ecliptic rotation matrix
    // for the given ecliptic to equatorial matrix
    // vector_ecliptic = vector_equatorial*matrix_equatorial_to_ecliptic
    const RotMatrix3x3& eqToEc(PrecType t, TimeUnit unit) const;

private:
    mutable pair<PrecType, RotMatrix3x3> _currentEcToEq, _currentEqToEc;

    static map<PrecType, RotMatrix3x3> _data;

    static void _initData();

};

}

#endif
