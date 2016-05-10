//# ElapsedTimeIntegrationEnder.h
//# Copyright (C) 2016 David Mehringer
//# 
//#     This program is free software: you can redistribute it and/or modify
//#     it under the terms of the GNU General Public License as published by
//#     the Free Software Foundation, either version 3 of the License, or
//#     (at your option) any later version.
//# 
//#     This program is distributed in the hope that it will be useful,
//#     but WITHOUT ANY WARRANTY; without even the implied warranty of
//#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#     GNU General Public License for more details.
//# 
//#     You should have received a copy of the GNU General Public License
//#     along with this program.  If not, see <http://www.gnu.org/licenses/>.
//# 
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
