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
