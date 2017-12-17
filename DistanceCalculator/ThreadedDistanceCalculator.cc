//# ThreadedDistanceCalculator.cc
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
 * ThreadedDistanceCalculator.cc
 *
 *  Created on: Sep 29, 2015
 *      Author: dmehring
 */

#include <DistanceCalculator/ThreadedDistanceCalculator.h>

#include <Math/Vector.h>

#include <future>

//#include <omp.h>

namespace kepler {
vector<Timer> ThreadedDistanceCalculator::timers(3);
vector<vector<Timer>> ThreadedDistanceCalculator::threadTimers(8, vector<Timer>(5));

ThreadedDistanceCalculator::ThreadedDistanceCalculator(
    int maxThreads, int nBodies
) : _maxThreads(min(maxThreads, nBodies*(nBodies-1)/2)),
        _bodyPairs(_maxThreads) ,_threads(_maxThreads)  {
    pair<int, int> p;
    int threadID = 0;
    for (int i=0; i<nBodies; ++i) {
        for (int j=i+1; j<nBodies; ++j) {
            _bodyPairs[threadID].push_back(pair<int, int>(i, j));
            ++threadID;
            if (threadID >= _maxThreads) {
                threadID = 0;
            }
        }
    }
}

ThreadedDistanceCalculator::ThreadedDistanceCalculator(
    int maxThreads, const vector<PrecType>& masses
) {
    auto c = std::count(begin(masses), end(masses), 0);
    int nBodies = masses.size();
    int npairs = nBodies*(nBodies-1)/2 - c*(c-1)/2;
    _maxThreads = min(maxThreads, npairs);
    _bodyPairs.resize(_maxThreads);
    _threads.resize(_maxThreads);
    int threadID = 0;
    for (int i=0; i<nBodies; ++i) {
        for (int j=i+1; j<nBodies; ++j) {
            if (masses[i] != 0 || masses[j] != 0) {
                _bodyPairs[threadID].push_back(pair<int, int>(i, j));
                ++threadID;
                if (threadID >= _maxThreads) {
                    threadID = 0;
                }
            }
        }
    }
}

ThreadedDistanceCalculator::~ThreadedDistanceCalculator() {}

void ThreadedDistanceCalculator::compute(
    DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const Vvector& x
) {
    _x = &x;
    int i = 0;
    _d = &d;
    _d2 = &d2;
    _dv = &dv;
    _diff = &diff;
    for (auto& t: _threads) {
        t.reset(new thread {&ThreadedDistanceCalculator::_compute, this, i});
        ++i;
    }
    for (auto& t: _threads) {
        t->join();
    }
    /*
    int n = _threads.size();
#pragma omp parallel for
    for (int i=0; i<n; ++i) {
        _compute(i); 
    }
    */
/*
    for (int i=0; i<n; ++i) {
        for (int j=i+1; j<n; ++j) {
            _compute(i,j);
        }
    }
    */
}

void ThreadedDistanceCalculator::_compute(
    int threadID
) {
    int i, j;
    for (const auto& bp: _bodyPairs[threadID]) {
        i = bp.first;
        j = bp.second;
        (*_x)[i].subtract((*_diff)[i][j], (*_x)[j]);
        (*_diff)[i][j].magAndmag2AndUnit((*_d)[i][j], (*_d2)[i][j], (*_dv)[i][j]);
        (*_d2)[j][i] = (*_d2)[i][j];
        (*_dv)[i][j].negate((*_dv)[j][i]);
    }
}
/*
void ThreadedDistanceCalculator::_compute(
    int i, int j
) {
    (*_x)[i].subtract((*_diff)[i][j], (*_x)[j]);
    (*_diff)[i][j].magAndmag2AndUnit((*_d)[i][j], (*_d2)[i][j], (*_dv)[i][j]);
    (*_d2)[j][i] = (*_d2)[i][j];
    (*_dv)[i][j].negate((*_dv)[j][i]);
}
*/
}
