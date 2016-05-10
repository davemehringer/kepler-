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

namespace kepler {
vector<Timer> ThreadedDistanceCalculator::timers(3);
vector<vector<Timer>> ThreadedDistanceCalculator::threadTimers(8, vector<Timer>(5));

ThreadedDistanceCalculator::ThreadedDistanceCalculator(
    int maxThreads, int nBodies
) : _maxThreads(min(maxThreads, nBodies/2*(nBodies-1))), _scratch(_maxThreads),
    _unit(_maxThreads), _x(), _bodyPairs(maxThreads), _biter(maxThreads),
    _bend(maxThreads),_threads(_maxThreads) {
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
    _tpairs.resize(_maxThreads);
    _td2.resize(_maxThreads);
    _tdv.resize(_maxThreads);
    int npairs = nBodies*(nBodies-1)/2;
    int nmin = npairs/_maxThreads;
    int nleft = npairs % _maxThreads;
    threadID = 0;
    //int count = 0;
    for (int i=0; i<nBodies; ++i) {
            for (int j=i+1; j<nBodies; ++j) {
                _tpairs[threadID].push_back(pair<int,int>(i, j));
                int s = _tpairs[threadID].size();
                if (nleft == 0 && s == nmin) {
                    ++threadID;
                }
                else if (s == nmin + 1) {
                    ++threadID;
                    --nleft;
                }
            }
    }
    auto vvd2 = begin(_td2);
    auto vvdv = begin(_tdv);
    for (const auto& t: _tpairs) {
        vvd2->resize(t.size());
        vvdv->resize(t.size());
        ++vvd2;
        ++vvdv;
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
}

void ThreadedDistanceCalculator::_compute(
    int threadID
) {
    int i, j;
    for (const auto& bp: _bodyPairs[threadID]) {
        i = bp.first;
        j = bp.second;
        _x->operator[](i).subtract(_diff->operator[](i)[j], _x->operator[](j));
        _diff->operator[](i)[j].magAndmag2AndUnit(_d->operator[](i)[j], _d2->operator[](i)[j], _dv->operator[](i)[j]);
        _d2->operator[](j)[i] = _d2->operator[](i)[j];
        _dv->operator[](i)[j].negate(_dv->operator[](j)[i]);
    }
}

}
