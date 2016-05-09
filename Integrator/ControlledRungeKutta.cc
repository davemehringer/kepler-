/*

 *      Author: dmehring
 */

#include <Integrator/ControlledRungeKutta.h>

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>
#include <IO/IO.h>
#include <Physics/NBodySystem.h>
#include <Integrator/PositionPublisher.h>

#include <iomanip>

using namespace std;
using namespace boost::numeric::odeint;

namespace kepler {

ControlledRungeKutta::ControlledRungeKutta(
    NBodySystem* system, AccelerationCalculator* accCalc,
    TimeStepManager *const &tsm,
    const IntegrationEnder *const& ie, PrecType err
) : ControlledIntegrator(system, accCalc, tsm, ie),
    _stepper(err, err )  {}

ControlledRungeKutta::~ControlledRungeKutta() {}

void ControlledRungeKutta::_step() {
    auto res = _stepper.try_step(coord(), _inState, _elapsedT, _outState, _deltaT);
    if (res == fail) {
        _step();
    }
}

}


