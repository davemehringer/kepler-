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
