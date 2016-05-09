/*
 * AccelerationTimeStepManager.cc
 *
 *  Created on: Sep 24, 2015
 *      Author: dmehring
 */

#include <TimeStepManager/AccelerationTimeStepManager.h>

namespace kepler {

AccelerationTimeStepManager::AccelerationTimeStepManager(
    PrecType aMin, PrecType aMax
) : TimeStepManager(), _aMin(aMin), _aMax(aMax) {}

AccelerationTimeStepManager::~AccelerationTimeStepManager() {}

}



