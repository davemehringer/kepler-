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
