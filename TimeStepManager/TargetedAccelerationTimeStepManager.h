//# TargetedAccelerationTimeStepManager.h
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
 *
 *      Author: dmehring
 */

#ifndef KEPLER_TARGETEDACCELERATIONTIMESTEPManager_H_
#define KEPLER_TARGETEDACCELERATIONTIMESTEPManager_H_

#include <IO/IO.h>
#include <Math/Vector.h>
#include <TimeStepManager/AccelerationTimeStepManager.h>

#include <iomanip>
#include <set>

using namespace std;

namespace kepler {

class TargetedAccelerationTimeStepManager: public AccelerationTimeStepManager {
public:

    // initialT and deltaT should be in seconds
    TargetedAccelerationTimeStepManager(
        PrecType aMin, PrecType aMax, const Vvector& x, const Vvector& v,
        Vvector& a, PrecType initialT
    );

    ~TargetedAccelerationTimeStepManager();

    // get the end time and the target bodies for the next step
    inline PrecType getNext(set<int>& targets, PrecType currentT) {

        const auto first = begin(_lastGoodStep);
        auto nextT = first->t + first->deltaT[0];
        //cout << "i " << 0 << setprecision(18) << " t  " << first->t << " dt " << first->deltaT[0] << endl;
        //cout << setprecision(18) << "current " << currentT << " next " << nextT << endl;

        if (_firstTime) {
            _firstTime = false;
            targets = _allBodies;
            return nextT;
        }
        auto iter = first;
        ++iter;
        auto myend = end(_lastGoodStep);

        int i = 1;
        for (; iter!=myend; ++iter, ++i) {
            //cout << "i " << i << setprecision(18) << " t  " << iter->t << " dt " << iter->deltaT[i] << endl;

            nextT = min(nextT, iter->t + iter->deltaT[i]);
            //cout << setprecision(18) << "current " << currentT << " next " << nextT << endl;
        }
        if (_decreased) {
            _targetBodies = _allBodies;
            _decreased = false;
        }
        else {
            _targetBodies.clear();
            i = 0;
            auto limit = 0.99 * /*_increaseFactor * */ (nextT - currentT);
            //cout << "limit " << limit << endl;
            for (const auto& lgs: _lastGoodStep) {
                //cout << "i " << i << " lgst " << lgs.t << " lgsdt " << lgs.deltaT[i]
                    //<< " nextT " << nextT << " limit " << limit << endl;
                if (lgs.t + lgs.deltaT[i] - nextT < limit) {
                    _targetBodies.insert(i);
                }
                ++i;
            }
        }
        //cout << "target bodies " << _targetBodies << endl;
        targets = _targetBodies;
        return nextT;
    }

    inline  TimeStepChangeType modify(
        const TimeStepManager::TimeStepManagerData& data
    ) {
        //cout << "initial " << *data.aInitial << endl;
        //cout << "final  " << *data.aFinal << endl;
        //auto afiter = begin(*data.aFinal);
        bool increase = false;
        bool decrease = false;
       // int i = 0;
        std::set<int> targets;
        //for (auto& a: *data.aInitial) {
        for (const auto& i: _targetBodies) {
            const auto& ai = data.aInitial->operator [](i);
            //afiter->subtract(_scratch,ai);
            const auto& af = data.aFinal->operator [](i);
            af.subtract(_scratch, ai);
            _scratch /= ai.mag();
            //cout << "i " << i << " frac a " << _scratch << endl;
            _scratch.sabs();
            auto m = _scratch.max();
            if (m > _aMax) {
                if (! decrease) {
                    targets.clear();
                }
                targets.insert(i);
                decrease = true;
                increase = false;
            }
            if (! decrease && m < _aMin) {
                    increase = true;
                    targets.insert(i);
            }
            //++afiter;
            //++i;
        }
        if (increase) {
            for (const auto& i: targets) {
                _lastGoodStep[i].deltaT[i] = _increaseFactor*(
                    data.currentTime + data.currentDeltaT  - _lastGoodStep[i].t
                );
                //cout << "i " << i << " new dt " << _lastGoodStep[i].deltaT[i]
               //       << " curT " << data.currentTime << " lastGoodTime "
                   //   << _lastGoodStep[i].t << endl;
                 if (i == 7) {
                     cout << "amalthea dt increase to " << _lastGoodStep[i].deltaT[i] << endl;
                 }
                 else if (i == 8) {
                     cout << "io dt increase to " << _lastGoodStep[i].deltaT[i] << endl;
                 }
            }
           // cout << "in modify: should increase " << endl;

            return TimeStepManager::INCREASE;
        }

        else if (decrease) {
            // find the earliest time, that is what we reset to
            int minI = 0;
            int j = 0;
            PrecType minT = 0;
            cout << "targets to decrease " << targets << endl;
            for (const auto& i: targets) {
                if (j == 0 || _lastGoodStep[i].t < minT) {
                    minT = _lastGoodStep[i].t;
                    minI = i;
                }
                ++j;
            }
            for (const auto& i: targets) {
                _lastGoodStep[minI].deltaT[i] /= _decreaseFactor;
                if (i == 7) {
                    cout << "amalthea dt decrease to " << _lastGoodStep[i].deltaT[i] << endl;
                }
                else if (i == 8) {
                    cout << "io dt decrease to " << _lastGoodStep[i].deltaT[i] << endl;
                }
            }
            fill(_lastGoodStep.begin(), _lastGoodStep.end(), _lastGoodStep[minI]);
            _decreased = true;
            return TimeStepManager::DECREASE;
        }
        return TimeStepManager::NO_CHANGE;
    }

    // clients should call this method after calling modify() if modify()
    // returns DECREASE, so they can reset their values to the previous
    // good step
    inline void reset(PrecType& t, Vvector& x, Vvector& v, Vvector& a) {
        // all of the elements of _lastGoodStep will have the same value
        // if the previous call to modify() returned DECREASE, so just use
        // the first element to get the data
        const auto& lgs = begin(_lastGoodStep);
        t = lgs->t;
        x = lgs->x;
        v = lgs->v;
        a = lgs->a;
    }

    // clients call this at the end of a successful step after they have updated
    // x, v, and a. Only the bodies stored in targetBodies will be updated.
    // It should be called before getNext() is called.
    inline void set(const Vvector& x, const Vvector& v, const Vvector& a, PrecType t) {
        for (const auto& i: _targetBodies) {
            _lastGoodStep[i].x = x;
            _lastGoodStep[i].v = v;
            _lastGoodStep[i].a = a;
            _lastGoodStep[i].t = t;
            for (const auto& j: _targetBodies) {
                if (i != j) {
                    _lastGoodStep[i].deltaT[j] = _lastGoodStep[j].deltaT[j];
                }
            }
        }
    }

private:
    struct LastGoodStep {
        Vvector x;
        Vvector v;
        Vvector a;
        // all bodies' deltaT's at the last good step
        vector<PrecType> deltaT;
        // time stamp of last good step
        PrecType t;
    };

    vector<LastGoodStep> _lastGoodStep;
    std::set<int> _targetBodies;
    std::set<int> _allBodies;
    bool _decreased = false;
    bool _firstTime = true;

};

}

#endif
