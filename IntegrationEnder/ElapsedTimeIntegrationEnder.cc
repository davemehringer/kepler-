/*
 * ElapsedTimeIntegrationEnder.cc
 *
 *  Created on: Sep 26, 2015
 *      Author: dmehring
 */

#include "ElapsedTimeIntegrationEnder.h"

namespace kepler {

ElapsedTimeIntegrationEnder::ElapsedTimeIntegrationEnder(PrecType maxElapsed)
    : _maxElapsed(maxElapsed*86400) {}

ElapsedTimeIntegrationEnder::~ElapsedTimeIntegrationEnder() {}

}
