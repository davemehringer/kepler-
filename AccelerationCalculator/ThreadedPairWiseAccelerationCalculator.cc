/*
 * ThreadedPairWiseAccelerationCalculator.cc
 *
 *  Created on: Sep 18, 2015
 *      Author: dmehring
 */

#include <AccelerationCalculator/ThreadedPairWiseAccelerationCalculator.h>

#include <Physics/Body.h>
#include <BodyFrameRotMatrix/BodyFrameRotMatrix.h>
#include <DistanceCalculator/DistanceCalculator.h>
//#include <IO/IO.h>

using namespace std;

namespace kepler {

ThreadedPairWiseAccelerationCalculator::ThreadedPairWiseAccelerationCalculator(
        DistanceCalculator* dc, const vector<Body> *const  &bodies, int nthreads
) : PairWiseAccelerationCalculator(dc, bodies),
    _nthreads(min(nthreads, (int)bodies->size())), _threadMap(_nthreads),
    _threads(_nthreads), _scratch(_nthreads) {
    auto n = bodies->size();
    auto bodiesPerThread = n/nthreads;
    auto left = n % nthreads;
    int threadID = 0;
    int i = 0;
    while (i < (int)n) {
        _threadMap[threadID].insert(i);
        if (_threadMap[threadID].size() == bodiesPerThread ) {
            if (left > 0) {
                ++i;
                _threadMap[threadID].insert(i);
                --left;
            }
            ++threadID;
        }
        ++i;
    }
}

ThreadedPairWiseAccelerationCalculator::~ThreadedPairWiseAccelerationCalculator() {}

void ThreadedPairWiseAccelerationCalculator::compute(
        Vvector& res, const Vvector *const  &x
) {
    _dc->compute(_d, _d2, _dv, _diff, *x);
    _a = &res;
    _compute(x);
}

void ThreadedPairWiseAccelerationCalculator::_compute(
    const Vvector *const  &x
) {
    int i = 0;
    for (auto s: _threadMap) {
        _threads[i].reset(
            new thread {
                &ThreadedPairWiseAccelerationCalculator::_compute2, this, s, x
            }
        );
        ++i;
    }
    for (auto& t: _threads) {
        t->join();
    }
}

void ThreadedPairWiseAccelerationCalculator::_compute2(
    const set<int>& idx, const Vvector *const  &x
) {
    int j = 0;
    for (auto i: idx) {
        const auto& body0 = _bodies->operator [](i);
        _a->operator [](i) = 0;
        auto scratch = _scratch[i];
        j = 0;
        for (const auto& body1: *_bodies) {
            if (i != j && body1.mu != 0) {
                // accumulate negative values because gravity is
                // an attractive force
                _dv[i][j].times(scratch, body1.mu/_d2[i][j]);
                _a->operator [](i) -= scratch;
                if (_hasJ[j] && body0.centerBody && body0.centerBody->id == body1.id && _d2[i][j] < _jRadLimit[j]) {
                    if (i > j) {
                        _d[i][j] = _d[j][i];
                        _diff[j][i].negate(_diff[i][j]);
                    }
                    _doJContrib(scratch, _d[i][j], _d2[i][j], _diff[i][j], body1);
                    _a->operator [](i) += scratch;
                }
            }
            ++j;
        }
    }
}

}
