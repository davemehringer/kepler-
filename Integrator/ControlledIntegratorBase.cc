//# ControlledIntegratorBase.cc
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

 *      Author: dmehring
 */


#include <Integrator/ControlledIntegratorBase.h>

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>
#include <IO/IO.h>
#include <Physics/NBodySystem.h>
#include <Integrator/PositionPublisher.h>

#include <iomanip>

using namespace std;
using namespace boost::numeric::odeint;

namespace kepler {

AccelerationCalculator* ControlledIntegratorBase::_aCalc = nullptr;
Vvector ControlledIntegratorBase::_xs;
Vvector ControlledIntegratorBase::_as;
//Timer ControlledIntegrator::_timer;

ControlledIntegratorBase::ControlledIntegratorBase(
    NBodySystem* system, AccelerationCalculator* accCalc,
    TimeStepManager *const &tsm,
    const IntegrationEnder *const& ie
) : Integrator(system, accCalc, tsm, ie),
        _inState(6*system->getNumberOfBodies())  {
    auto n = system->getNumberOfBodies();
    _xs.resize(n);
    _as.resize(n);
    _ai.resize(n);
    _af.resize(n);
    Vvector x(n), v(n);
    system->getXV(x, v);
    _populateFirst(_inState, x);
    _populateSecond(_inState, v);
    _outState = _inState;
    _aCalc = accCalc;
}

ControlledIntegratorBase::~ControlledIntegratorBase() {}

void ControlledIntegratorBase::integrate() {
    _accCalc->setTime(_startTime, SECOND);
    _accCalc->compute(_ai, &_x);
     _tsmd.aInitial = &_ai;
     _tsmd.currentTime = _currentTime;
     _tsmd.currentDeltaT = _deltaT;
     auto updateStep = _pp ? _pp->getNSteps() : 0;
     while(! _ie->end(_ied)) {
        _ie->setNextDeltaT(_deltaT, _ied);
        _step();
        _inState = _outState;
        ++_nsteps;
        if (_pp && _nsteps % updateStep == 0) {
            _populateFromFirst(_x, _outState);
            _pp->setX(_x);
        }
    }
    if (_pp) {
        _pp->end();
    }
    _populateFromFirst(_x, _outState);
    _populateFromSecond(_v, _outState);
    //cout << "SB timer duration " << _timer.totalDuration() << endl;
}

}


