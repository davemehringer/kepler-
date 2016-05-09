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



