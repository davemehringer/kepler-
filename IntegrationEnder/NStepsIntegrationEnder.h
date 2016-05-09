/*
 * NStepsIntegrationEnder.h
 *
 *  Created on: Sep 26, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_NSTEPSINTEGRATIONENDER_H_
#define KEPLER_NSTEPSINTEGRATIONENDER_H_

#include "IntegrationEnder.h"

namespace kepler {

class NStepsIntegrationEnder: public IntegrationEnder {
public:
    NStepsIntegrationEnder(int maxSteps);

    ~NStepsIntegrationEnder();

    inline bool end(const IntegrationEnderData& ied) const {
        return *ied.nsteps >= _maxSteps;
    }

    inline void setNextDeltaT(PrecType& deltaT, const IntegrationEnderData& ied) const {}

private:
    int _maxSteps = 0;
};

}

#endif
