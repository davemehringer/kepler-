/*
 * SerialTargetedAccelerationCalculator.cc
 *
 *  Created on: Sep 18, 2015
 *      Author: dmehring
 */

#include <AccelerationCalculator/SerialTargetedAccelerationCalculator.h>

#include <Physics/Body.h>
#include <BodyFrameRotMatrix/BodyFrameRotMatrix.h>
#include <IO/IO.h>
#include <DistanceCalculator/SerialTargetedDistanceCalculator.h>

using namespace std;

namespace kepler {

SerialTargetedAccelerationCalculator::SerialTargetedAccelerationCalculator(
    SerialTargetedDistanceCalculator* dc, const vector<Body> *const  &bodies
) : PairWiseAccelerationCalculator(dc, bodies), _sdc(dc) {
    vector<int> x(bodies->size());
    iota(x.begin(), x.end(), 0);
   _allBodies.reset(new set<int>(x.begin(), x.end()));
   _targets = _allBodies.get();
   dc->setTargets(_targets);
}

SerialTargetedAccelerationCalculator::~SerialTargetedAccelerationCalculator() {}

void SerialTargetedAccelerationCalculator::compute(
        Vvector& res, const Vvector *const  &x
) {
    _sdc->compute(_d, _d2, _dv, _diff, *x);
    _compute(res, x);
}

void SerialTargetedAccelerationCalculator::setTargetBodies(
        const set<int>* targets
) {
    _sdc->setTargets(targets);
    _targets = targets;
}

void SerialTargetedAccelerationCalculator::_compute(
    Vvector& res, const Vvector *const  &x
) {
    int j = 0;
    for (const auto& i: *_targets) {
        const auto& body0 = _bodies->operator [](i);
        res[i] = 0;
        j = 0;
        for (const auto& body1: *_bodies) {
            if (i != j) {
                // accumulate negative values because gravity is
                // an attractive force
                _dv[i][j].times(_scratch, body1.mu/_d2[i][j]);
                res[i] -= _scratch;
                if (_hasJ[j] && body0.centerBody && body0.centerBody->id == body1.id && _d2[i][j] < _jRadLimit[j]) {
                    _doJContrib(_scratch, _d[i][j], _d2[i][j], _diff[i][j], body1);
                    res[i] += _scratch;
                }
            }
            ++j;
        }
    }
}

}
