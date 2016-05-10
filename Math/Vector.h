//# Vector.h
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
#ifndef __KEPLER_VECTOR
#define __KEPLER_VECTOR

#include <Math/Types.h>

#include <algorithm>
#include <array>
#include <iostream>

using namespace std;

namespace kepler {

// Representation of a physical vector.

class Vector {

    friend void times(Vector& res, const Vector& v, PrecType scalar);

public:
	// A 0 length Vector
	Vector();

	Vector(PrecType x, PrecType y, PrecType z);

	Vector(const std::array<PrecType, 3>& v);

	~Vector() {}

	inline Vector& operator=(const Vector& other) {
	    if (this != &other) {
	        // don't copy _scratch
	        _components = other._components;
	    }
	    return *this;
	}

	// no index checking is done
	inline const PrecType& operator[](int i) const {
		return _components[i];
	}

	inline PrecType& operator[](int i) {
	    //_mag = 0;
	    //_mag2 = 0;
		return _components[i];
	}

	// set all components equal to the specified scalar
	inline Vector& operator=(PrecType scalar) {
	        std::fill(
	            _components.begin(), _components.end(), scalar
	        );
	        return *this;
	    }

	// add the specified vector from this vector
	inline Vector operator+(const Vector& v) const {
		std::transform(
			_components.begin(), _components.end(), v._components.begin(),
			_scratch.begin(), std::plus<PrecType>()
		);
		return _scratch;
	}

	inline void plus(Vector& res, const Vector& v) const {
	    std::transform(
	        _components.begin(), _components.end(), v._components.begin(),
	        res._components.begin(), std::plus<PrecType>()
	    );
	}

	// subtract the specified vector from this vector
	inline Vector operator-(const Vector& v) const {
		std::transform(
			_components.begin(), _components.end(), v._components.begin(),
			_scratch.begin(), std::minus<PrecType>()
		);
		return _scratch;
	}

    inline void minus(Vector& res, const Vector& v) const {
        std::transform(
            _components.begin(), _components.end(), v._components.begin(),
            res._components.begin(), std::minus<PrecType>()
        );
    }

	// multiply all the components by the scalar value
	inline Vector operator*(PrecType scalar) const {
		std::transform(
			_components.begin(), _components.end(), _scratch.begin(),
			bind2nd(multiplies<PrecType>(), scalar)
		);
		return _scratch;
	}

	inline Vector& operator*=(PrecType scalar) {
	    std::transform(
	        _components.begin(), _components.end(), _components.begin(),
	        bind2nd(multiplies<PrecType>(), scalar)
	    );
	    return *this;
	}

	// divide all the components by the scalar value
	inline Vector operator/(PrecType scalar) const {
		std::transform(
			_components.begin(), _components.end(), _scratch.begin(),
			bind2nd(divides<PrecType>(), scalar)
		);
		return _scratch;
	}

	inline void divide(Vector& res, PrecType scalar) const {
	        std::transform(
	            _components.begin(), _components.end(), res._components.begin(),
	            bind2nd(divides<PrecType>(), scalar)
	        );
	    }

	inline Vector& operator/=(PrecType scalar) {
	    std::transform(
	        _components.begin(), _components.end(), _components.begin(),
	        bind2nd(divides<PrecType>(), scalar)
	    );
	    return *this;
	}

	inline Vector& operator+=(const Vector& v) {
	    //_mag = 0;
	    //_mag2 = 0;
		std::transform(
			_components.begin(), _components.end(), v._components.begin(),
			_components.begin(), std::plus<PrecType>()
		);
		return *this;
	}

	inline Vector& operator-=(const Vector& v) {
	    //_mag = 0;
	    //_mag2 = 0;
		std::transform(
			_components.begin(), _components.end(), v._components.begin(),
			_components.begin(), std::minus<PrecType>()
		);
		return *this;
	}

    // subtract the specified vector from this vector
    inline void subtract(Vector& res, const Vector& v) const {
        std::transform(
            _components.begin(), _components.end(), v._components.begin(),
            res._components.begin(), std::minus<PrecType>()
        );
    }

	inline void times(Vector& res, PrecType scalar) const {
	    std::transform(
	        _components.begin(), _components.end(), res._components.begin(),
	        bind2nd(multiplies<PrecType>(), scalar)
	    );
	}

	bool operator==(const Vector& v) const;

	// cross product.
	inline Vector cross(const Vector& v) const {
	    _scratch[0] = _components[1]*v._components[2] - _components[2]*v._components[1];
        _scratch[1] = _components[2]*v._components[0] - _components[0]*v._components[2];
        _scratch[2] = _components[0]*v._components[1] - _components[1]*v._components[0];
        return _scratch;
	}

	inline const array<PrecType, 3>& data() const { return _components; }

	// dot product of this vector with the input vector
	inline PrecType dot(const Vector& v) const {
		std::transform(
			_components.begin(), _components.end(), v._components.begin(),
			_scratch.begin(), std::multiplies<PrecType>()
		);
		return std::accumulate(_scratch.begin(), _scratch.end(), 0.0);
	}

	// compute the magnitude of this vector
	inline PrecType mag() const {
	    return sqrt(mag2());
	}

	// compute the square of the magnitude of this vector
	inline PrecType mag2() const {
	    return dot(*this);
	}

	inline PrecType max() const {
	    return *max_element(_components.begin(), _components.end());
	}

	// negate this vector (multiply by -1)
	inline Vector negate() const {
		return operator*(-1.0);
	}

	inline void negate(Vector& res) const {
	    this->times(res, -1.0);
	}

	inline void sabs() {
	    for (auto& c: _components) {
	        c = abs(c);
	    }
	}

	inline Vector unit() const {
		return operator/(mag());
	}

	// save a calculation by getting all at once
	inline void magAndmag2AndUnit(PrecType& mag, PrecType& mag2, Vector& unit) {
	    mag2 = dot(*this);
	    mag = sqrt(mag2);
	    std::transform(
	        _components.begin(), _components.end(), unit._components.begin(),
	        bind2nd(divides<PrecType>(), mag)
	    );
	}

	inline void unit(Vector& res) const {
	    divide(res, mag());
	}

private:

	array<PrecType, 3> _components;
	mutable array<PrecType, 3> _scratch;
};

ostream& operator<<(ostream& os, const Vector& v);

inline Vector operator*(PrecType d, const Vector& v) {
	return v*d;
}

// returns true if the relative magnitudes are within epsilon of each
// other and the angle separating them is >= delta radians
bool near(const Vector&u, const Vector& v, PrecType epsilon, PrecType delta);

inline void times(Vvector& res, const Vvector& v, PrecType scalar) {
    auto riter = begin(res);
    for (auto& vec: v) {
        vec.times(*riter, scalar);
        ++riter;
    }
}

inline void add(Vvector& res, const Vvector& u, const Vvector& v) {
    auto riter = begin(res);
    auto viter = begin(v);
    for (auto& vec: u) {
        vec.plus(*riter, *viter);
        ++riter;
        ++viter;
    }
}

inline void subtract(Vvector& res, const Vvector& u, const Vvector& v) {
    auto riter = begin(res);
    auto viter = begin(v);
    for (auto& vec: u) {
        vec.minus(*riter, *viter);
        ++riter;
        ++viter;
    }
}
// get the square of the magnitudes of all Vectors

inline vector<PrecType> mag2(const Vvector& v) {
    vector<PrecType> mag2(v.size());
    auto iter = begin(mag2);
    for (auto vec: v) {
        *iter = vec.mag2();
    }
    return mag2;
}


inline void mag2(vector<PrecType>& mag2, const Vvector& v) {
    auto iter = begin(mag2);
    for (auto vec: v) {
        *iter = vec.mag2();
    }
}

inline Vvector operator*(PrecType d, const Vvector& v) {
    Vvector res(v.size(), Vector());
    auto iter = begin(res);
    for (const auto& vv: v) {
        vv.times(*iter, d);
        ++iter;
    }
    return res;
}

inline Vvector operator+(const Vvector& u, const Vvector& v) {
    Vvector res(v.size(), Vector());
    auto uiter = begin(u);
    auto viter = begin(v);
    for (auto& rr: res) {
        uiter->plus(rr, *viter);
        ++uiter;
        ++viter;
    }
    return res;
}

}

#endif

