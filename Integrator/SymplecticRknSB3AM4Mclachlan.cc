//# SymplecticRknSB3AM4Mclachlan.cc
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

 *      Author: dmehring
 */

#include <Integrator/SymplecticRknSB3AM4Mclachlan.h>

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>
#include <IO/IO.h>
#include <Physics/NBodySystem.h>
#include <Integrator/PositionPublisher.h>

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
