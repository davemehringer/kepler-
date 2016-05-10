//# StateVectorsOrbitalElements.h
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
#ifndef KEPLER_STATEVECTORSORBITALELEMENTS_H_
#define KEPLER_STATEVECTORSORBITALELEMENTS_H_

#include <Math/Types.h>

#include <iostream>
#include <utility>

using namespace std;

namespace kepler {

class Vector;

// angles should be in degrees
struct Elements {
    // semi-major axis
    PrecType a;
    // eccentricity
    PrecType e;
    // inclination
    PrecType i;
    // longitude of ascending node
    PrecType node;
    // argument of pericenter
    PrecType peri;
    // mean anomaly
    PrecType mAnomaly;
};

// the first Vector in the returned pair is the position, the
// second is the velocity. These are relative to the center of
// the body being orbited.
pair<Vector, Vector> elementsToVectors(
    PrecType muPrimary, PrecType muSecondary, const Elements& elements
);

Elements vectorsToElements(
    PrecType muPrimary, PrecType muSecondary, const Vector& x, const Vector& v
);

bool near(const Elements& e1, const Elements& e2, PrecType epsilon);

ostream& operator <<(ostream& os, const Elements& e);

}
#endif
