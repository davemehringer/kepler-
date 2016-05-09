/*
 * ElapsedTimeIntegrationEnder.h
 *      Author: dmehring
 */

#ifndef KEPLER_ELAPSEDTIMEINTEGRATIONENDER_H_
#define KEPLER_ELAPSEDTIMEINTEGRATIONENDER_H_

#include "IntegrationEnder.h"

#include <iostream>

using namespace std;

namespace kepler {

class ElapsedTimeIntegrationEnder: public IntegrationEnder {
public:
    // max elapsed time in days
    ElapsedTimeIntegrationEnder(PrecType maxElapsedTime);

    ~ElapsedTimeIntegrationEnder();

    inline bool end(const IntegrationEnderData& ied) const {
        return *ied.elapsedTime >= _maxElapsed;
    }

    inline void setNextDeltaT(PrecType& deltaT, const IntegrationEnderData& ied) const {
        deltaT = min(deltaT, _maxElapsed - *ied.elapsedTime);
    }


private:
    PrecType _maxElapsed = 0;
};

}

#endif
