//# TargetedAccelerationTimeStepManager.cc
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



