//# Vector.cc
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
#include "Vector.h"

#include "Math.h"

#include <iomanip>

namespace kepler {

	Vector::Vector() : _components {0, 0, 0} /*, _mag(0), _mag2(0) */  {}

	Vector::Vector(PrecType x, PrecType y, PrecType z)
		: _components {x, y, z} /*, _mag(0), _mag2(0) */ {}

	Vector::Vector(const std::array<PrecType, 3>& v)
	: _components(v) /*, _mag(0), _mag2(0) */ {}

	bool Vector::operator==(const Vector& v) const {
		return _components == v._components;
	}

	ostream& operator<<(ostream& os, const Vector& v) {
	    os << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]";
	    return os;
	}

	bool near(const Vector&u, const Vector& v, PrecType epsilon, PrecType delta) {
	    if (u == v) {
	        return true;
	    }
	    auto umag = u.mag();
	    auto vmag = v.mag();
	    if (! near(umag, vmag, epsilon)) {
	        return false;
	    }
	    auto c = u.dot(v)/umag/vmag;
	    if (c > 1) {
	        // this can happen because of finite precision issues
	        return true;
	    }
	    return acos(c) <= delta;
	}


}
