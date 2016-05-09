/*

 *      Author: dmehring
 */

#ifndef KEPLER_symplectic_rkn_sb3a_m4_mclachlan_H
#define KEPLER_symplectic_rkn_sb3a_m4_mclachlan_H

#include <Integrator/Symplectic.h>

#include <boost/numeric/odeint/stepper/symplectic_rkn_sb3a_m4_mclachlan.hpp>

using namespace boost::numeric::odeint;

namespace kepler {

class SymplecticRknSB3AM4Mclachlan: public Symplectic {
public:

    using stepper_type = symplectic_rkn_sb3a_m4_mclachlan<container_type>;


    SymplecticRknSB3AM4Mclachlan() = delete;

    SymplecticRknSB3AM4Mclachlan(
        NBodySystem* system, AccelerationCalculator* accCalc,
        TimeStepManager *const &tsm,
        const IntegrationEnder *const& ie
    );

    virtual ~SymplecticRknSB3AM4Mclachlan();

protected:
    stepper_type _stepper;

    virtual void _step();

};

}

#endif
