//# Timer.h
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
#ifndef KEPLER_TIMER_H
#define KEPLER_TIMER_H

#include <chrono>

using namespace std::chrono;

namespace kepler {

class Timer {
    // <summary>
    // Timer based on C++11 chrono library
    // </summary>

    // <reviewed reviewer="" date="" tests="" demos="">
    // </reviewed>

    // <prerequisite>
    // </prerequisite>

    // <etymology>
    // Timer based on C++11 chrono library
    // </etymology>

    // <synopsis>
    // Timer based on C++11 chrono library
    // </synopsis>

public:
    // create the timer but do not start it.
    Timer();

    ~Timer();

    // return mean duration, in seconds, of all start/stop cycles
    double meanDuration() const;

    // duration in seconds of most recent start/stop cycle
    double duration() const;

    // number of start/stop cycles
    int nCycles() const;

    // (re)start the timer
    void start();

    // stop the timer
    void stop();

    // total duration in seconds, sum of all start/stop cycles
    // doesn't include the duration of the current cycle if the
    // timer is currently running
    double totalDuration() const;

private:
    steady_clock::time_point _start;
    std::chrono::duration<double> _duration, _totalDuration;
    int _nCycles;
};
}

#endif

