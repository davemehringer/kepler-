//# Integrator.h
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
 * Integrator.h
 *
 *  Created on: Sep 20, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_INTEGRATOR_H_
#define KEPLER_INTEGRATOR_H_

#include <Math/Constants.h>
#include <Math/Types.h>
#include <IntegrationEnder/IntegrationEnder.h>
#include <TimeStepManager/TimeStepManager.h>

namespace kepler {

class AccelerationCalculator;
class NBodySystem;
class PositionPublisher;
class TimeStepManager;

class Integrator {
public:

    Integrator() = delete;

    virtual ~Integrator();

    PrecType getElapsedTime(TimeUnit unit) const {
        if (unit == SECOND) {
            return _elapsedT;
        }
        else {
            return _elapsedT/SECOND_PER_DAY;
        }
    }

    void getXV(Vvector& x, Vvector&v) const { x = _x; v= _v; }

    virtual void integrate() = 0;

    int nsteps() const {
        return _nsteps;
    }

    void setDeltaT(PrecType dt) { _deltaT = dt; }

    void setPositionPublisher(PositionPublisher *const &pp);

protected:
    const NBodySystem *const _system = nullptr;
    AccelerationCalculator* _accCalc = nullptr;
    TimeStepManager *const _tsm;
    TimeStepManager::TimeStepManagerData _tsmd;
    TimeStepManager::TimeStepChangeType _tsct = TimeStepManager::NO_CHANGE;
    const IntegrationEnder *const _ie = nullptr;
    IntegrationEnder::IntegrationEnderData _ied = IntegrationEnder::IntegrationEnderData();
    PrecType _deltaT;
    PrecType _elapsedT = 0;
    PrecType _startTime = 0;
    PrecType _currentTime = 0;
    Vvector _x, _v, _a;
    int _nsteps = 0;
    PositionPublisher *_pp = nullptr;

    Integrator(
        NBodySystem* system, AccelerationCalculator* accCalc,
        TimeStepManager *const &tsm,
        const IntegrationEnder *const& ie
    );

private:
    void _init();

};

}

#endif
