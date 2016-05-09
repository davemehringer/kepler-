/*
 * ThreadedPairWiseAccelerationCalculator.h
 *
 *  Created on: Sep 18, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_ThreadedPairWiseAccelerationCalculator_H_
#define KEPLER_ThreadedPairWiseAccelerationCalculator_H_

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>

#include <set>
#include <memory>
#include <thread>

using namespace std;

namespace kepler {

// compute acceleration of each body based on forces due to
// all other individual bodies

class ThreadedPairWiseAccelerationCalculator : public PairWiseAccelerationCalculator {
public:

    ThreadedPairWiseAccelerationCalculator() = delete;

    ThreadedPairWiseAccelerationCalculator(
        DistanceCalculator* dc, const vector<Body> *const  &bodies,
        int nThreads
    );

    ~ThreadedPairWiseAccelerationCalculator();

    void compute(Vvector& res, const Vvector *const  &x);

private:
    int _nthreads = 1;
    vector<set<int>> _threadMap;
    Vvector  *_a = nullptr;
    vector<unique_ptr<thread>> _threads;
    mutable Vvector _scratch;

    void _compute(const Vvector *const  &x);

    void _compute2(const set<int>& idx, const Vvector *const  &x);

};

}

#endif
