//# NStepsIntegrationEnder.h
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

    inline void setNextDeltaT(PrecType& , const IntegrationEnderData& ) const {}

private:
    int _maxSteps = 0;
};

}

#endif
