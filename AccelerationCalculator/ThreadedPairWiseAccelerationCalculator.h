//# ThreadedPairWiseAccelerationCalculator.h
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
