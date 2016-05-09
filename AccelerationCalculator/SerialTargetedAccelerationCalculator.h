/*
 * SerialTargetedAccelerationCalculator.h
 *
 *      Author: dmehring
 */

#ifndef KEPLER_SerialTargetedACCELERATIONCALCULATOR_H_
#define KEPLER_SerialTargetedACCELERATIONCALCULATOR_H_

#include "PairWiseAccelerationCalculator.h"

#include <memory>
#include <set>

using namespace std;

namespace kepler {

class SerialTargetedDistanceCalculator;

// compute acceleration of selected bodies based on forces due to
// all other individual bodies

class SerialTargetedAccelerationCalculator : public PairWiseAccelerationCalculator {
public:

    SerialTargetedAccelerationCalculator() = delete;

    SerialTargetedAccelerationCalculator(
        SerialTargetedDistanceCalculator* sdc, const vector<Body> *const  &bodies
    );

    virtual ~SerialTargetedAccelerationCalculator();

    // only the corresponding Vectors for the target bodies will be
    // updated in res.
    virtual void compute(Vvector& res, const Vvector *const  &x);

    // these are the bodies for which accelerations will be computed
    // upon the next call to compute().
    void setTargetBodies(const set<int>* targets);

protected:

    virtual void _compute(Vvector& res, const Vvector *const  &x);

private:
    SerialTargetedDistanceCalculator* _sdc;
    mutable Vector _scratch;
     const set<int> *_targets = nullptr;
     unique_ptr<set<int>> _allBodies;
};

}

#endif
