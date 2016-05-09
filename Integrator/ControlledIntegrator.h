/*

 *      Author: dmehring
 */

#ifndef KEPLER_CONTROLLEDINTEGRATOR_H
#define KEPLER_CONTROLLEDINTEGRATOR_H

#include <Integrator/ControlledIntegratorBase.h>

#include <IO/IO.h>
#include <OS/Timer.h>

#include <AccelerationCalculator/AccelerationCalculator.h>

#include <boost/numeric/odeint.hpp>

#include <Math/Vector.h>

using namespace boost::numeric::odeint;

namespace kepler {

using container_type = Vvector;

template <class StepperType> class ControlledIntegrator: public ControlledIntegratorBase {
public:

    using State = vector<PrecType>;

    ControlledIntegrator() = delete;

    ControlledIntegrator(
        NBodySystem* system, AccelerationCalculator* accCalc,
        TimeStepManager *const &tsm,
        const IntegrationEnder *const& ie, StepperType  stepper
    );

    virtual ~ControlledIntegrator();

protected:

    virtual void _step();

private:
    StepperType _stepper;

};

}
#include <Integrator/ControlledIntegrator.tcc>
#endif
