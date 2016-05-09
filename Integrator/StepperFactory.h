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
