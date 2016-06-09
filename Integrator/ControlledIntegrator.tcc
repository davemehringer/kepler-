/*

 *      Author: dmehring
 */

#include <Integrator/ControlledIntegrator.h>

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>
#include <IO/IO.h>
#include <Physics/NBodySystem.h>
#include <Integrator/PositionPublisher.h>

#include <iomanip>

using namespace std;
using namespace boost::numeric::odeint;

namespace kepler {

template <class StepperType>
ControlledIntegrator<StepperType>::ControlledIntegrator(
    NBodySystem* system, AccelerationCalculator* accCalc,
    TimeStepManager *const &tsm,
    const IntegrationEnder *const& ie, StepperType stepper
) : ControlledIntegratorBase(system, accCalc, tsm, ie),
    _stepper(stepper) {}

template <class StepperType> ControlledIntegrator<StepperType>::~ControlledIntegrator() {}

template <class StepperType> void ControlledIntegrator<StepperType>::_step() {
    auto res = _stepper->try_step(coord(), _inState, _elapsedT, _outState, _deltaT);
    if (res == fail) {
        _step();
    }
}

}

