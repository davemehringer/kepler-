//# PositionPublisher.h
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
 * PositionPublisher.h
 *
 *  Created on: Oct 9, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_POSITIONPUBLISHER_H_
#define KEPLER_POSITIONPUBLISHER_H_

#include <Math/Types.h>
#include <Physics/Body.h>

namespace kepler {

// publish position updates using zeromq to subscribers

class PositionPublisher {
public:
    // if the bodies vector is not empty, the body names will be written
    // immediately to the fifo
    PositionPublisher(const vector<Body>& bodies = vector<Body>());

    ~PositionPublisher();

    // signal clients we are done
    void end();

    int getNSteps() const { return _nsteps; }

    // update the plot every nSteps
    void setNSteps(int n) { _nsteps = n; }

    // set the position data to be published immediately
    void setX(const Vvector& x);

    bool canWrite() const { return _canWrite; }

private:
    int _writefd = -1;
    int _nsteps = 100;
    bool _canWrite = false;
    bool _done = false;

};

}

#endif
