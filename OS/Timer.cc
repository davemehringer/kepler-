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


