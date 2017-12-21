//# RungeKutta.cc
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
 * RungeKutta.cc
 *
 *  Created on: Sep 20, 2015
 *      Author: dmehring
 */

#include <Integrator/RungeKutta.h>

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>
#include <IO/IO.h>
#include <Physics/NBodySystem.h>
#include <Integrator/PositionPublisher.h>

using namespace std;

namespace kepler {

RungeKutta::RungeKutta(
    NBodySystem* system, AccelerationCalculator* accCalc,
    TimeStepManager *const &tsm,
    const IntegrationEnder *const& ie
) : Integrator(system, accCalc, tsm, ie) {
    int n = _system->getBodies().size();
    _delX1.resize(n);
    _x1.resize(n);
    _delV1.resize(n);
    _v1.resize(n);
    _delX2.resize(n);
    _x2.resize(n);
    _delV2.resize(n);
    _v2.resize(n);
     _delX3.resize(n);
     _x3.resize(n);
     _delV3.resize(n);
     _v3.resize(n);
     _delX4.resize(n);
     _delV4.resize(n);
     _delX.resize(n);
     _delV.resize(n);
     _xFinal.resize(n);
     _vFinal.resize(n);
     _a1.resize(n);
     _a2.resize(n);
     _a3.resize(n);
     _aFinal.resize(n);
     _a.resize(n);
}

RungeKutta::~RungeKutta() {}

void RungeKutta::integrate() {
    _accCalc->setTime(_startTime, SECOND);
    _accCalc->compute(_a, &_x);
    _tsmd.aInitial = &_a;
    _tsmd.aFinal = &_aFinal;
    _tsmd.currentTime = _currentTime;
    _tsmd.currentDeltaT = _deltaT;
    auto updateStep = _pp ? _pp->getNSteps() : 0;
    while(! _ie->end(_ied)) {
        _step();
        if (_pp && _nsteps % updateStep == 0) {
            _pp->setX(_x);
        }
        ++_nsteps;
    }
    if (_pp) {
        _pp->end();
    }
    cout << "current " << _currentTime << " start " << _startTime << endl;
}

void RungeKutta::updateXVAT() {
    _a =_aFinal;
    _x = _xFinal;
    times(_delV2, _delV2, 2);
    times(_delV3, _delV3, 2);
    add(_delV, _delV1, _delV2);
    add(_delV, _delV, _delV3);
    add(_delV, _delV, _delV4);
    times(_delV, _delV, 1/6.0);
    add(_v, _v, _delV);
    _currentTime += _deltaT;
    _elapsedT = _currentTime - _startTime;
}

void RungeKutta::_step() {
    _deltaT = _tsm->getDeltaT();
    _ie->setNextDeltaT(_deltaT, _ied);
    _rkStep();
    _tsct = _tsm->modify(_tsmd);
    if (_tsct == TimeStepManager::DECREASE) {
        _step();
        return;
    }
    updateXVAT();
}

void RungeKutta::_rkStep() {
    _accCalc->setTime(_currentTime, SECOND);

    times(_delX1, _v, _deltaT);
    times(_x1, _delX1, 0.5);
    add(_x1, _x1, _x);
    times(_delV1, _a, _deltaT);
    times(_v1, _delV1, 0.5);
    add(_v1, _v1, _v);

    _accCalc->compute(_a1, &_x1);
    times(_delX2, _v1, _deltaT);
    times(_x2, _delX2, 0.5);
    add(_x2, _x2, _x);
    times(_delV2, _a1, _deltaT);
    times(_v2, _delV2, 0.5);
    add(_v2, _v2, _v);

    _accCalc->compute(_a2, &_x2);
    times(_delX3, _v2, _deltaT);
    add(_x3, _x, _delX3);
    times(_delV3, _a2, _deltaT);
    add(_v3, _v, _delV3);

    _accCalc->compute(_a3, &_x3);
    times(_delX4, _v3, _deltaT);
    times(_delV4, _a3, _deltaT);

    times(_delX2, _delX2, 2);
    times(_delX3, _delX3, 2);
    add(_delX, _delX1, _delX2);
    add(_delX, _delX, _delX3);
    add(_delX, _delX, _delX4);
    times(_delX, _delX, 1/6.0);
    add(_xFinal, _x, _delX);
    _accCalc->compute(_aFinal, &_xFinal);
}

}


