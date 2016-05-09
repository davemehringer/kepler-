/*
 * UnthreadedDistanceCalculator.h
 *
 *  Created on: Sep 29, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_SERIALTARGETEDDISTANCECALCULATOR_H_
#define KEPLER_SERIALTARGETEDDISTANCECALCULATOR_H_

#include <DistanceCalculator/DistanceCalculator.h>

#include <OS/Timer.h>

#include <set>
#include <memory>

using namespace std;

namespace kepler {

// compute distances only for selected bodies. If a body is selected
// (in the target list), the distances between it and all other bodies
// are computed

class SerialTargetedDistanceCalculator: public DistanceCalculator {
public:

    static Timer _timer;

    SerialTargetedDistanceCalculator();

    ~SerialTargetedDistanceCalculator();

    // d2 is the distance squared matrix. dv is the distance components matrix.
    // Note that for performance reasons, because only small number of values
    // of d and diff are required by callers, only values for these variables are
    // returned for when the first index is less than the second. It is up to the
    // caller to make the necessary transformation d[j][i] -> d[i][j] and diff[j][i] ->
    // diff[i][j] when i > j. Note that only entries for target indices will be updated.
    // When passed in, d should be all zeros
    void compute(
        DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const Vvector& x
    );

    void setTargets(const set<int>* targets) { _targets = targets; }

private:
    Vvector::const_iterator  /*_xiter,*/ _yiter, _xend;
    int _i = 0;
    int _j = 0;
   // Vector _unit, _scratch;
   const set<int> * _targets = nullptr;
   set<int>::iterator _titer, _tend, _viter;
};

}

#endif
