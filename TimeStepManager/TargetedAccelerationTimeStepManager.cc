/*
 * AccelerationTimeStepManager.cc
 *
 *  Created on: Sep 24, 2015
 *      Author: dmehring
 */

#include <TimeStepManager/TargetedAccelerationTimeStepManager.h>

namespace kepler {

TargetedAccelerationTimeStepManager::TargetedAccelerationTimeStepManager(
    PrecType aMin, PrecType aMax, const Vvector& x, const Vvector& v,
    Vvector& a, PrecType initialT
) : AccelerationTimeStepManager(aMin, aMax), _lastGoodStep(), _targetBodies() {
    auto n = x.size();
    vector<PrecType> deltaT(n, _deltaT);
    LastGoodStep lgs;
    lgs.x = x;
    lgs.v = v;
    lgs.a = a;
    lgs.deltaT = deltaT;
    lgs.t = initialT;
    _lastGoodStep.resize(n, lgs);
    vector<int> targets(n);
    iota(targets.begin(), targets.end(), 0);
    _allBodies.insert(targets.begin(), targets.end());
    _targetBodies = _allBodies;
}

TargetedAccelerationTimeStepManager::~TargetedAccelerationTimeStepManager() {}

}



