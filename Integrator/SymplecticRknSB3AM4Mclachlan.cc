/*

 *      Author: dmehring
 */

#include <Integrator/SymplecticRknSB3AM4Mclachlan.h>

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>
#include <IO/IO.h>
#include <Physics/NBodySystem.h>
#include <Integrator/PositionPublisher.h>

#include <iomanip>

using namespace std;
using namespace boost::numeric::odeint;

namespace kepler {

SymplecticRknSB3AM4Mclachlan::SymplecticRknSB3AM4Mclachlan(
    NBodySystem* system, AccelerationCalculator* accCalc,
    TimeStepManager *const &tsm,
    const IntegrationEnder *const& ie
) : Symplectic(system, accCalc, tsm, ie), _stepper() {}

SymplecticRknSB3AM4Mclachlan::~SymplecticRknSB3AM4Mclachlan() {}

void SymplecticRknSB3AM4Mclachlan::_step() {
    _stepper.do_step(_odeSystem , _inState, _elapsedT, _outState,   _deltaT );
}

}


