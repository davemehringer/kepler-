//# AccelerationTimeStepManager.h
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
 * AccelerationTimeStepManager.h
 *
 *  Created on: Sep 23, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_ACCELERATIONTIMESTEPManager_H_
#define KEPLER_ACCELERATIONTIMESTEPManager_H_

#include <Math/Vector.h>
#include <TimeStepManager/TimeStepManager.h>

namespace kepler {

class AccelerationTimeStepManager: public TimeStepManager {
public:
    AccelerationTimeStepManager(PrecType aMin, PrecType aMax);

    virtual ~AccelerationTimeStepManager();

    virtual string className() const { return "AccelerationTimeStepManager"; }

    inline virtual  TimeStepChangeType modify(
        const TimeStepManager::TimeStepManagerData& data
    ) {
        auto afiter = begin(*data.aFinal);
        bool increase = true;
        for (auto& a: *data.aInitial) {
            afiter->subtract(_scratch, a);
            _scratch /= a.mag();
            _scratch.sabs();
            auto m = _scratch.max();
            if (m > _aMax) {
                _deltaT /= _decreaseFactor;
                return TimeStepManager::DECREASE;
            }
            if (increase && m > _aMin) {
                    increase = false;
            }
            ++afiter;
        }
        if (increase) {
            _deltaT *= _increaseFactor;
            return TimeStepManager::INCREASE;
        }
        return TimeStepManager::NO_CHANGE;
    }

protected:
    PrecType _aMin, _aMax;
    mutable Vector _scratch = Vector();

};

}

#endif
