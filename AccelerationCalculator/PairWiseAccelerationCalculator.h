//# PairWiseAccelerationCalculator.h
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
 * PairWiseAccelerationCalculator.h
 *
 *  Created on: Sep 18, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_PAIRWISEACCELERATIONCALCULATOR_H_
#define KEPLER_PAIRWISEACCELERATIONCALCULATOR_H_

#include <AccelerationCalculator/AccelerationCalculator.h>

#include <Math/Vector.h>

#include <vector>

using namespace std;

namespace kepler {

class RotMatrix3x3;

// compute acceleration of each body based on forces due to
// all other individual bodies

class PairWiseAccelerationCalculator : public AccelerationCalculator {
public:

    PairWiseAccelerationCalculator() = delete;

    PairWiseAccelerationCalculator(
        DistanceCalculator* dc, const vector<Body> *const  &bodies
    );

    virtual ~PairWiseAccelerationCalculator();

    virtual void compute(Vvector& res, const Vvector *const  &x);

protected:
    const vector<Body> * const _bodies;
    DMatrix _d, _d2;
    VMatrix _dv, _diff;
    vector<bool> _hasJ;
    vector<PrecType> _jRadLimit;

    void _compute(Vvector& res, const Vvector *const  &x);

private:
        mutable Vector _scratch;



};

}

#endif
