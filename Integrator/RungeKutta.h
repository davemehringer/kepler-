//# RungeKutta.h
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
 * RungeKutta.h
 *
 *  Created on: Sep 20, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_RUNGEKUTTA_H_
#define KEPLER_RUNGEKUTTA_H_

#include <Integrator/Integrator.h>

#include <Math/Vector.h>

namespace kepler {

// Runge-Kutta numerical integrator.
class RungeKutta: public Integrator {
public:

    RungeKutta() = delete;

    RungeKutta(
        NBodySystem* system, AccelerationCalculator* accCalc,
        TimeStepManager *const &tsm,
        const IntegrationEnder *const& ie
    );

    virtual ~RungeKutta();

    virtual void integrate();

protected:
    Vvector _delX1, _x1, _delV1, _v1;
    Vvector _delX2, _x2, _delV2, _v2;
    Vvector _delX3, _x3, _delV3, _v3;
    Vvector _delX4, _delV4;
    Vvector _delX, _delV;
    Vvector _xFinal, _vFinal;
    Vvector _a, _a1, _a2, _a3, _aFinal;

    virtual void _step();

    void _rkStep();

    void updateXVAT();
};

}

#endif
