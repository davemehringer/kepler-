/*
 * StepperFactory.cc
 *
 *  Created on: Nov 8, 2015
 *      Author: dmehring
 */

#include <Integrator/StepperFactory.h>

namespace kepler {

BSStepperType StepperFactory::createBurlirschStoer(
    PrecType maxAbsErr, PrecType maxRelErr
) {
    return BSStepperType(
        new bulirsch_stoer<vector<PrecType>, PrecType>(maxAbsErr, maxRelErr)
    );
}

RKCashKarpStepperType StepperFactory::createRKCashKarp(
    PrecType maxAbsErr, PrecType maxRelErr
) {
    auto stepper = make_controlled(maxAbsErr, maxRelErr, runge_kutta_cash_karp54< vector<PrecType>, PrecType>());
    return RKCashKarpStepperType(
        new controlled_runge_kutta<runge_kutta_cash_karp54< vector<PrecType>, PrecType>>(stepper)
    );
}

RKFehlbergStepperType StepperFactory::createRKFehlberg(
    PrecType maxAbsErr, PrecType maxRelErr
) {
    auto stepper = make_controlled(maxAbsErr, maxRelErr, runge_kutta_fehlberg78<vector<PrecType>, PrecType>());
    return RKFehlbergStepperType(
        new controlled_runge_kutta<runge_kutta_fehlberg78< vector<PrecType>, PrecType>>(stepper)
    );
}

}
