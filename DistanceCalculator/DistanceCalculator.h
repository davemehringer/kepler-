//# DistanceCalculator.h
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
 * DistanceCalculator.h
 *
 *  Created on: Sep 29, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_DISTANCECALCULATOR_H_
#define KEPLER_DISTANCECALCULATOR_H_

#include <Math/Types.h>

namespace kepler {

class DistanceCalculator {
public:

    virtual ~DistanceCalculator() {}

    // d2 is the distance squared matrix. dv is the distance components matrix
    // d is the distance matrix, diff is the position difference components matrix
    virtual void compute(
        DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const Vvector& x
    ) = 0;

    /*
    virtual void compute(
            DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const vector<PrecType>& x
    ) = 0;
*/
protected:
    DistanceCalculator() {}

};

}

#endif
