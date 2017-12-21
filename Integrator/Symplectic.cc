//# Symplectic.cc
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

#include <Integrator/Symplectic.h>

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>
#include <IO/IO.h>
#include <Physics/NBodySystem.h>
#include <Integrator/PositionPublisher.h>

using namespace std;
using namespace boost::numeric::odeint;

namespace kepler {

Symplectic::Symplectic(
    NBodySystem* system, AccelerationCalculator* accCalc,
    TimeStepManager *const &tsm,
    const IntegrationEnder *const& ie
) : Integrator(system, accCalc, tsm, ie),
    _odeSystem(
        make_pair(
            coord(system->getMasses() ) , momentum(_accCalc, system->getMasses())
     )
    ) {
           auto n = system->getNumberOfBodies();
           _ai.resize(n);
           _af.resize(n);
        Vvector p(system->getNumberOfBodies());
        Vvector q(system->getNumberOfBodies());
        auto bodies = system->getBodies();
        auto piter = begin(p);
        auto qiter = begin(q);
        for (const auto& body: bodies) {
            *piter = body.mu/G*body.v;
            *qiter = body.x;
            ++piter;
            ++qiter;
        }
        _inState = make_pair(q, p);
        _outState = _inState;
}

Symplectic::~Symplectic() {}

void Symplectic::integrate() {
    _accCalc->setTime(_startTime, SECOND);
    _accCalc->compute(_ai, &_x);
    _odeSystem.second._a = _ai;
    _tsmd.aFinal = &_odeSystem.second._a;
    _tsmd.aInitial = &_ai;
    cout << "initial " << _tsmd.aInitial << " final " << _tsmd.aFinal << endl;
    _tsmd.currentTime = _currentTime;
    _tsmd.currentDeltaT = _deltaT;
    auto updateStep = _pp ? _pp->getNSteps() : 0;
    while(! _ie->end(_ied)) {
        _deltaT = _tsm->getDeltaT();
        _ie->setNextDeltaT(_deltaT, _ied);
        _step();
        _accCalc->setTime(_currentTime, SECOND);
        _accCalc->compute(*_tsmd.aFinal, &_outState.first);
        _tsct = _tsm->modify(_tsmd);
        if (_tsct == TimeStepManager::DECREASE) {
            _step();
        }
        else if (_tsct == TimeStepManager::INCREASE) {
            cout << "Increased dt to " << _tsm->getDeltaT() << endl;
        }
        _inState = _outState;
        _elapsedT += _deltaT;
        _currentTime += _deltaT;
        _ai = *_tsmd.aFinal;
        if (_pp && _nsteps % updateStep == 0) {
            _pp->setX(_outState.first);
        }
        ++_nsteps;
    }
    if (_pp) {
        _pp->end();
    }
    _x = _outState.first;
    auto m = _system->getMasses();
    auto miter = begin(m);
    auto piter = begin(_outState.second);
    for (auto& v: _v) {
        v = *piter/(*miter);
        ++miter;
        ++piter;
    }
}

}


