//# Timer.cc
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
#include "Timer.h"

using namespace std::chrono;

namespace kepler {
Timer::Timer()
    : _start(), _duration(), _totalDuration(), _nCycles(0) {}

Timer::~Timer() {}

double Timer::duration() const {
    return _duration.count();
}

double Timer::meanDuration() const {
    return totalDuration()/nCycles();
}

int Timer::nCycles() const {
    return _nCycles;
}

void Timer::start() {
    _start = steady_clock::now();
}

void Timer::stop() {
    _duration = steady_clock::now() - _start;
    _totalDuration += _duration;
    ++_nCycles;
}

double Timer::totalDuration() const {
    return _totalDuration.count();
}

}


