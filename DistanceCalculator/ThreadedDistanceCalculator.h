//# ThreadedDistanceCalculator.h
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
 * UnthreadedDistanceCalculator.h
 *
 *  Created on: Sep 29, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_THREADEDDISTANCECALCULATOR_H_
#define KEPLER_THREADEDDISTANCECALCULATOR_H_

#include <DistanceCalculator/DistanceCalculator.h>

#include <OS/Timer.h>

#include <memory>
#include <thread>

using namespace std;

namespace kepler {

class ThreadedDistanceCalculator: public DistanceCalculator {
public:

        static vector<Timer> timers;
        static vector<vector<Timer>> threadTimers;


    ThreadedDistanceCalculator() = delete;

    ThreadedDistanceCalculator(int maxThreads, int nBodies);

    ~ThreadedDistanceCalculator();

    // d2 is the distance squared matrix. dv is the distance components matrix
    // Note that for performance reasons, because only small number of values
    // of d and diff are required by callers, only values for these variables are
    // returned for when the first index is less than the second. It is up to the
    // caller to make the necessary transformation d[j][i] -> d[i][j] and diff[j][i] ->
    // diff[i][j] when i > j
    void compute(DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const Vvector& x);

private:
        int _maxThreads;
    vector<Vector>  _scratch, _unit;
    const Vvector* _x;
    vector<vector<pair<int, int>>> _bodyPairs;
    vector<vector<pair<int, int>>::const_iterator> _biter, _bend;
    vector<unique_ptr<thread>> _threads;
    DMatrix *_d2 = nullptr, *_d = nullptr;
    VMatrix *_dv = nullptr, *_diff = nullptr;

    vector<vector<PrecType>> _td2;
    vector<vector<Vector>> _tdv;
    vector<vector<pair<int, int>>> _tpairs;

    // threaded
    void _compute(int threadID);

    vector<pair<PrecType, Vector>> _compute2(int threadID);

    void _compute3(int threadID);

};

}

#endif
