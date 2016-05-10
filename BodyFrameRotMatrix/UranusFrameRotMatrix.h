//# UranusFrameRotMatrix.h
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
 * BodyFrameRotMatrix.h
 *
 *  Created on: Sep 8, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_UranusFRAMEROTMATRIX_H_
#define KEPLER_UranusFRAMEROTMATRIX_H_

#include <BodyFrameRotMatrix/BodyFrameRotMatrix.h>

#include <Math/RotMatrix3x3.h>

#include <map>

using namespace std;

namespace kepler {

class UranusFrameRotMatrix: public BodyFrameRotMatrix {
public:

    UranusFrameRotMatrix();

    ~UranusFrameRotMatrix();

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
