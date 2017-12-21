//# Integrator.cc
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
 * Integrator.cc
 *
 *  Created on: Sep 24, 2015
 *      Author: dmehring
 */

#include "Integrator.h"

#include <AccelerationCalculator/AccelerationCalculator.h>
#include <Integrator/PositionPublisher.h>
#include <Physics/NBodySystem.h>

namespace kepler {

Integrator::Integrator(
       NBodySystem* system, AccelerationCalculator* accCalc,
       TimeStepManager *const &tsm,
       const IntegrationEnder *const& ie
) : _system(system), _accCalc(accCalc), _tsm(tsm),
    _ie(ie), _deltaT(tsm->getDeltaT()), _startTime(system->getTime(SECOND)),
    _currentTime(_startTime) {
   _init();
}

Integrator::~Integrator() {}

void Integrator::_init() {
    auto n = _system->getBodies().size();
    _x.resize(n);
    _v.resize(n);
    _a.resize(n);
    _system->getXV(_x, _v);
    _accCalc->setTime(_startTime, SECOND);
    _accCalc->compute(_a, &_x);
    _tsmd.aInitial = &_a;
    _ied.elapsedTime = &_elapsedT;
    _ied.nsteps = &_nsteps;
}

void Integrator::setPositionPublisher(
    PositionPublisher *const &pp
) {
    _pp = pp && pp->canWrite() ? pp : nullptr;
}

}



