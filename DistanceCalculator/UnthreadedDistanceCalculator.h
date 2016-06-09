//# UnthreadedDistanceCalculator.h
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
 * UnthreadedDistanceCalculator.h
 *
 *  Created on: Sep 29, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_UNTHREADEDDISTANCECALCULATOR_H_
#define KEPLER_UNTHREADEDDISTANCECALCULATOR_H_

#include <DistanceCalculator/DistanceCalculator.h>

#include <OS/Timer.h>
#include <Math/Vector.h>

namespace kepler {

class UnthreadedDistanceCalculator: public DistanceCalculator {
public:

    static Timer _timer;

    // masses is used to determine for what pairs distances are computed.
    // If both masses in a pair are zero, their distance is not computed because
    // the accelerations of one due to the other will not be computed (because
    // it is always 0).
    UnthreadedDistanceCalculator(const vector<PrecType>& masses = vector<PrecType>());

    ~UnthreadedDistanceCalculator();

    // d2 is the distance squared matrix. d is the distance matrix,
    // dv is the unit vector difference components matrix, diff is the
    // difference components matrix. Because d and diff are only used
    // in a small number of cases, these matrices are populated only for
    // i<j. It is up to the caller to perform the trivial transformations
    // d[j][i] = d[i][j] and diff[j][i] = -diff[i][j]

    void compute(
        DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const Vvector& x
    );

private:
    Vvector::const_iterator _xiter, _yiter, _xend;
    int _i = 0;
    int _j = 0;
    Vector _unit, _scratch;
    vector<bool> _positiveMass;
    vector<bool>::const_iterator _p1, _p2;
    bool _hasZeroMassPair;

     void _computeWithZeroMassPairs(
        DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const Vvector& x
    );
};



}

#endif
