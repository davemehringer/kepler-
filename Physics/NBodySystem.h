//# NBodySystem.h
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
#ifndef __KEPLER_NBODYSYSTEM
#define __KEPLER_NBODYSYSTEM

#include <Math/Constants.h>
#include <Math/Types.h>
#include <Physics/Body.h>

namespace kepler {

class NBodySystem {

public:

    // a copy of bodies is created so that their positions and velocities
    // can be adjusted relative to the center of mass of the system
    NBodySystem(const vector<Body>& bodies, PrecType startTime, TimeUnit unit);

    Vector getAngularMomentum() const;

    size_t getNumberOfBodies() const { return _bodies.size(); }

    const vector<Body>& getBodies() const { return _bodies; }

    vector<PrecType> getMasses() const;

    PrecType getEnergy() const {return _kineticEnergy() + _potenialEnergy(); }

    // x and v must have the correct size when passed in.
    inline void getXV(Vvector& x, Vvector& v) const {
        auto xiter = begin(x);
        auto viter = begin(v);
        for (const auto& body: _bodies) {
            *xiter = body.x;
            *viter = body.v;
            ++xiter;
            ++viter;
        }
    }

    inline void step(
        const vector<Vector>& delX, const vector<Vector>& delV,
        PrecType delT, TimeUnit unit
    ) {
        auto biter = begin(_bodies);
        auto xiter = begin(delX);
        auto viter = begin(delV);
        auto bend = end(_bodies);
        for ( ; biter != bend; ++biter, ++xiter, ++viter) {
            biter->x += *xiter;
            biter->v += *viter;
        }
        if (unit == DAY) {
            delT *= SECOND_PER_DAY;
        }
        _time += delT;
    }

    // get the current system time in the specified unit
    PrecType getTime(TimeUnit unit) const;

    void setTime(PrecType t, TimeUnit unit);

    void setXV(const Vvector& x, const Vvector& v);

private:

    vector<Body> _bodies;
    // time is stored in seconds
    PrecType _time;
    vector<PrecType> _masses;

    PrecType _kineticEnergy() const;

    PrecType _potenialEnergy() const;

};

ostream& operator<<(ostream& os, const NBodySystem& v);


}
   
#endif 
