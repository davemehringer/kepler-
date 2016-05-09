/*
 * IntegrationEnder.h
 *
 *  Created on: Sep 25, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_INTEGRATIONENDER_H_
#define KEPLER_INTEGRATIONENDER_H_

#include <Math/Types.h>

namespace kepler {

class IntegrationEnder {
public:

    struct IntegrationEnderData {
        PrecType* elapsedTime;
        int* nsteps;
        // debugging
        bool forceEnd;
    };

    virtual ~IntegrationEnder();

    virtual bool end(const IntegrationEnderData& ied) const = 0;

    virtual void setNextDeltaT(PrecType& deltaT, const IntegrationEnderData& ied) const = 0;

protected:
    IntegrationEnder();

};

}

#endif
