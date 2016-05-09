/*

 *      Author: dmehring
 */

#include <Integrator/SymplecticRknSB3AMclachlan.h>

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>
#include <IO/IO.h>
#include <Physics/NBodySystem.h>
#include <Integrator/PositionPublisher.h>

#include <iomanip>

using namespace std;
using namespace boost::numeric::odeint;

namespace kepler {

SymplecticRknSB3AMclachlan::SymplecticRknSB3AMclachlan(
    NBodySystem* system, AccelerationCalculator* accCalc,
    TimeStepManager *const &tsm,
    const IntegrationEnder *const& ie
) : Symplectic(system, accCalc, tsm, ie), _stepper() {}

SymplecticRknSB3AMclachlan::~SymplecticRknSB3AMclachlan() {}

void SymplecticRknSB3AMclachlan::_step() {
    _stepper.do_step(_odeSystem , _inState, _elapsedT, _outState,   _deltaT );
}

}


