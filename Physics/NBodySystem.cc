//# NBodySystem.cc
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
#include <Physics/NBodySystem.h>

#include <Exception/KeplerException.h>
#include <Math/Vector.h>

#include <algorithm>

namespace kepler {

NBodySystem::NBodySystem(
    const vector<Body>& bodies, PrecType startTime, TimeUnit unit
) : _bodies(bodies),
    _time(unit == SECOND ? startTime : startTime*SECOND_PER_DAY),
    _masses(_bodies.size()) {
    Vector xcm(0, 0, 0);
    Vector vcm(0, 0, 0);
    PrecType muTot = 0;
    PrecType mu = 0;

    auto i = 0;
    for (const auto& body: _bodies) {
        mu = body.mu;
        _masses[i] = body.mu/G;
        muTot += mu;
        xcm += mu * body.x;
        vcm += mu * body.v;
        ++i;
    }
    xcm /= muTot;
    vcm /= muTot;
    for (auto& body: _bodies) {
        body.x -= xcm;
        body.v -= vcm;
        body.origin = Origin::SYSTEM_BARYCENTER;
    }
}

Vector NBodySystem::getAngularMomentum() const {
    Vector angmom;
    for (const auto& body: _bodies) {
        angmom += body.mu * body.x.cross(body.v);
    }
    return angmom;
}

vector<PrecType> NBodySystem::getMasses() const {
    return _masses;
}

PrecType NBodySystem::getTime(TimeUnit unit) const {
    if (unit == DAY) {
        return _time/SECOND_PER_DAY;
    }
    return _time;
}

void NBodySystem::setTime(PrecType t, TimeUnit unit) {
    _time = unit == DAY ? t*SECOND_PER_DAY : t;
}

PrecType NBodySystem::_kineticEnergy() const {
    PrecType ke = 0;
    for (const auto& body: _bodies) {
        ke += body.mu * body.v.mag2();
    }
    ke *= 0.5;
    return ke;
}

PrecType NBodySystem::_potenialEnergy() const {
    PrecType pe = 0;
    auto iter = begin(_bodies);
    auto iend = end(_bodies);
    auto jiter = iter;
    for (; iter!=iend; ++iter) {
        jiter = iter;
        ++jiter;
        for (; jiter!=iend; ++jiter) {
            auto diff = iter->x - jiter->x;
            pe -= iter->mu * jiter->mu/diff.mag();
        }
    }
    return pe;
}

void NBodySystem::setXV(const Vvector& x, const Vvector& v) {
    auto xiter = begin(x);
    auto viter = begin(v);
    for (auto& body: _bodies) {
        body.x = *xiter;
        body.v = *viter;
        ++viter;
        ++xiter;
    }
}

ostream& operator<<(ostream& os, const NBodySystem& system) {
    os << "system time: " << system.getTime(DAY) << "days" << endl;
    auto& bodies = system.getBodies();
    cout << "number of bodies " << bodies.size() << endl;
    for (const auto& body: bodies) {
        os << body << endl;
    }
    return os;
}

}

