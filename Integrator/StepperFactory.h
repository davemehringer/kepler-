//# StepperFactory.h
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
 * StepperFactory.h
 *
 *  Created on: Nov 8, 2015
 *      Author: dmehring
 */

#ifndef SRC_INTEGRATOR_STEPPERFACTORY_H_
#define SRC_INTEGRATOR_STEPPERFACTORY_H_

#include <Math/Types.h>

#include <boost/numeric/odeint.hpp>

#include <memory>

using namespace std;

using namespace boost::numeric::odeint;

namespace kepler {

using BSStepperType = shared_ptr<bulirsch_stoer<vector<PrecType>, PrecType>>;

using RKCashKarpStepperType = shared_ptr<controlled_runge_kutta<runge_kutta_cash_karp54<vector<PrecType>, PrecType>>>;

using RKFehlbergStepperType = shared_ptr<controlled_runge_kutta<runge_kutta_fehlberg78< vector<PrecType>, PrecType>>>;

class StepperFactory {

public:

    static BSStepperType createBurlirschStoer(
        PrecType maxAbsErr, PrecType maxRelErr
    );

    static  RKCashKarpStepperType createRKCashKarp(
        PrecType maxAbsErr, PrecType maxRelErr
    );

    static RKFehlbergStepperType  createRKFehlberg(
        PrecType maxAbsErr, PrecType maxRelErr
    );

};

}

#endif
