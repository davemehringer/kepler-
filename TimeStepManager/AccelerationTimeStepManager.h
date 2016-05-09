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
