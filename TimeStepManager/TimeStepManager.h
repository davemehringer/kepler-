/*
 * TimeStepManager.h
 *
 *  Created on: Sep 23, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_TIMESTEPManager_H_
#define KEPLER_TIMESTEPManager_H_

#include <Math/Types.h>

#include <string>

namespace kepler {

class TimeStepManager {

public:
    struct TimeStepManagerData {
        Vvector* aInitial;
        Vvector* aFinal;
        PrecType currentTime;
        PrecType currentDeltaT;
    } ;

    enum TimeStepChangeType {
        NO_CHANGE,
        INCREASE,
        DECREASE
    };

    virtual ~TimeStepManager() {}

    virtual string className() const { return "TimeStepManager"; }

    virtual  TimeStepChangeType modify(
        const TimeStepManagerData& data
    ) = 0;
/*
    virtual  TimeStepChangeType modify(
        PrecType& deltaT, const TimeStepManagerData& data
    ) const = 0;
*/
    PrecType decreaseFactor() const { return _decreaseFactor; }

    PrecType increaseFactor() const { return _increaseFactor; }

    void setDecreaseFactor(PrecType f) { _decreaseFactor = f; }

    void setIncreaseFactor(PrecType f) { _increaseFactor = f; }

    inline const PrecType& getDeltaT() const { return _deltaT; }

protected:
    PrecType _decreaseFactor = 2;
    PrecType _increaseFactor = 2;
    PrecType _deltaT = 1;

    TimeStepManager()  {}
};

}

#endif
