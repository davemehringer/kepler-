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
