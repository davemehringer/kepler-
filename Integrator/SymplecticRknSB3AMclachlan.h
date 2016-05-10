//# SymplecticRknSB3AMclachlan.h
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

#ifndef KEPLER_symplectic_rkn_sb3a_mclachlan_H
#define KEPLER_symplectic_rkn_sb3a_mclachlan_H

#include <Integrator/Symplectic.h>

namespace kepler {

class SymplecticRknSB3AMclachlan: public Symplectic {
public:
    using stepper_type = symplectic_rkn_sb3a_mclachlan<container_type>;

    SymplecticRknSB3AMclachlan() = delete;

    SymplecticRknSB3AMclachlan(
        NBodySystem* system, AccelerationCalculator* accCalc,
        TimeStepManager *const &tsm,
        const IntegrationEnder *const& ie
    );

    virtual ~SymplecticRknSB3AMclachlan();

protected:
    stepper_type _stepper;

    virtual void _step();

};

}

#endif
