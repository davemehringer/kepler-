//# TargetedRungeKutta.h
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
 * TargetedRungeKutta.h
 *
 *      Author: dmehring
 */

#ifndef KEPLER_TargetedRungeKutta_H_
#define KEPLER_TargetedRungeKutta_H_

#include <Integrator/RungeKutta.h>

#include <Math/Vector.h>

#include <set>

using namespace std;

namespace kepler {

class SerialTargetedAccelerationCalculator;
class TargetedAccelerationTimeStepManager;

// Runge-Kutta numerical integrator.
class TargetedRungeKutta: public RungeKutta {
public:

    TargetedRungeKutta() = delete;

    TargetedRungeKutta(
        NBodySystem* system, SerialTargetedAccelerationCalculator* accCalc,
        TargetedAccelerationTimeStepManager *const tatsm,
        const IntegrationEnder *const ie
    );

    ~TargetedRungeKutta();

protected:

    void _step();

private:
    SerialTargetedAccelerationCalculator* _stac;
    TargetedAccelerationTimeStepManager *const _tatsm;
    set<int> _targets;

};

}

#endif
