/*
 * RungeKutta.cc
 *
 *  Created on: Sep 20, 2015
 *      Author: dmehring
 */

#include <Integrator/TargetedRungeKutta.h>

#include <IO/IO.h>
#include <Physics/NBodySystem.h>
#include <AccelerationCalculator/SerialTargetedAccelerationCalculator.h>
#include <Integrator/PositionPublisher.h>
#include <TimeStepManager/TargetedAccelerationTimeStepManager.h>

namespace kepler {

TargetedRungeKutta::TargetedRungeKutta(
    NBodySystem* system, SerialTargetedAccelerationCalculator* accCalc,
    TargetedAccelerationTimeStepManager *const tatsm,
    const IntegrationEnder *const ie
) : RungeKutta(system, accCalc, tatsm, ie), _stac(accCalc), _tatsm(tatsm) {}

TargetedRungeKutta::~TargetedRungeKutta() {}

void TargetedRungeKutta::_step() {
    _deltaT = _tatsm->getNext(_targets, _currentTime) - _currentTime;
   // cout << "_deltaT " << _deltaT << " step " << _nsteps << endl;
   // cout << "step " << _nsteps << " deltaT " << _deltaT << endl;
    _stac->setTargetBodies(&_targets);
    _rkStep();
    //cout << "before call to modify" << endl;
    _tsmd.currentTime = _currentTime;
    _tsmd.currentDeltaT = _deltaT;
    _tsct = _tatsm->modify(_tsmd);
    if (_tsct == TimeStepManager::DECREASE) {
     //   cout << "decrease at step " << _nsteps << endl;
      //  cout << setprecision(16) << "current time before reset " << _currentTime << endl;
        _tatsm->reset(_currentTime, _x, _v, _a);
       // _elapsedT = _currentTime - _startTime;
        /*
        cout << "t reset to " << _currentTime << endl;
        cout << "x reset to " << _x << endl;
        cout << "v reset to " << _v << endl;
        cout << "a reset to " << _a << endl;
        cout << setprecision(16) << "current time after  reset " << _currentTime << endl;
        // first return is for debug
        _ied.forceEnd = true;
        return;
        */
        _step();
        return;
    }
    updateXVAT();
    _tatsm->set(_x, _v, _a, _currentTime);
    /*
    cout << setprecision(16) << "t set to " << _currentTime << endl;
    cout << "x set to " << _x << endl;
    cout << "v set to " << _v << endl;
    cout << "a set to " << _a << endl;
    */
   // _elapsedT += _deltaT;

}

}


