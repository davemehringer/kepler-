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

    UnthreadedDistanceCalculator();

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

    /*
    // for boost steppers
    virtual void compute(
            DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const vector<PrecType>& q
    );
*/
private:
    Vvector::const_iterator _xiter, _yiter, _xend;
    int _i = 0;
    int _j = 0;
    Vector _unit, _scratch;
};

}

#endif
