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
    _stepper(stepper)  {}

template <class StepperType> ControlledIntegrator<StepperType>::~ControlledIntegrator() {}

template <class StepperType> void ControlledIntegrator<StepperType>::_step() {
    //cout << "start step" << endl;
    //cout << "in " << _inState << endl;
    //cout << "t " << _elapsedT << " dt " << _deltaT << endl;

    auto res = _stepper->try_step(coord(), _inState, _elapsedT, _outState, _deltaT);
    if (res == fail) {
        //cout << "fail" << endl;
        //cout << "in " << _inState << endl;
        //cout << "out " << _outState << endl;
        _step();
    }
    auto xdiff = _outState[3] - _outState[6];
    auto ydiff = _outState[4] - _outState[7];
    cout  << fixed << setw(10) << setprecision(0) << _elapsedT
            << " " << setw(12) << setprecision(0) << sqrt(xdiff*xdiff + ydiff*ydiff)
            << " " << setw(8) << setprecision(3) <<  _deltaT << endl;
}

}


