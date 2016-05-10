//# ControlledRungeKutta.h
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

#ifndef KEPLER_CONTROLLEDRUNGEKUTTA_H
#define KEPLER_CONTROLLEDRUNGEKUTTA_H

#include <IO/IO.h>
#include <OS/Timer.h>

#include <AccelerationCalculator/AccelerationCalculator.h>

#include <boost/numeric/odeint.hpp>

#include <Math/Vector.h>
#include "ControlledIntegratorBase.h"

using namespace boost::numeric::odeint;

namespace kepler {

using container_type = Vvector;

class ControlledRungeKutta: public ControlledIntegrator {
public:

    using State = vector<PrecType>;

    ControlledRungeKutta() = delete;

    ControlledRungeKutta(
        NBodySystem* system, AccelerationCalculator* accCalc,
        TimeStepManager *const &tsm,
        const IntegrationEnder *const& ie, PrecType err
    );

    virtual ~ControlledRungeKutta();

protected:

    virtual void _step();

private:
    controlled_runge_kutta< State > _stepper;

};

}

#endif
