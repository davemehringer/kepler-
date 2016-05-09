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
   vector< bool> _hasJ;
    vector<PrecType> _jRadLimit;


    virtual void _compute(Vvector& res, const Vvector *const  &x);
/*
    void _doJContrib(
        Vector& aj, PrecType d, PrecType d2, const Vector& diff, const Body& body
    );
*/
private:
        mutable Vector _scratch;



};

}

#endif
