//# UnthreadedDistanceCalculator.cc
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
 * UnthreadedDistanceCalculator.cc
 *
 *  Created on: Sep 29, 2015
 *      Author: dmehring
 */

#include <DistanceCalculator/UnthreadedDistanceCalculator.h>

namespace kepler {
Timer UnthreadedDistanceCalculator::_timer;
UnthreadedDistanceCalculator::UnthreadedDistanceCalculator(
    const vector<PrecType>& masses
) : _positiveMass(masses.size()), _hasZeroMassPair(false) {
    auto p = begin(_positiveMass);
    for (const auto& m: masses) {
        *p = (m != 0);
        ++p;
    }
    _hasZeroMassPair = std::count(begin(_positiveMass), end(_positiveMass), false) >= 2;
}

UnthreadedDistanceCalculator::~UnthreadedDistanceCalculator() {}

void UnthreadedDistanceCalculator::compute(
    DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const Vvector& x
) {
    if (_hasZeroMassPair) {
        _computeWithZeroMassPairs(d, d2, dv, diff, x);
        return;
    }
    _xiter = begin(x);
    _xend = end(x);
    _i = 0;
    for ( ; _xiter != _xend; ++_xiter, ++_i) {
        _j = _i + 1;
        _yiter = _xiter;
        ++_yiter;
        for ( ; _yiter != _xend; ++_yiter, ++_j) {
            _xiter->subtract(diff[_i][_j], *_yiter);
            diff[_i][_j].magAndmag2AndUnit(d[_i][_j], d2[_i][_j], dv[_i][_j]);
            d2[_j][_i] = d2[_i][_j];
            dv[_i][_j].negate(dv[_j][_i]);
        }
    }
}

void UnthreadedDistanceCalculator::_computeWithZeroMassPairs(
       DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const Vvector& x
) {
    _xiter = begin(x);
    _xend = end(x);
    _p1 = begin(_positiveMass);
    _i = 0;
    for ( ; _xiter != _xend; ++_xiter, ++_i, ++_p1) {
        _j = _i + 1;
        _yiter = _xiter;
        ++_yiter;
        _p2 = _p1;
        ++_p2;
        for ( ; _yiter != _xend; ++_yiter, ++_j, ++_p2) {
            if (*_p1 || *_p2) {
                _xiter->subtract(diff[_i][_j], *_yiter);
                diff[_i][_j].magAndmag2AndUnit(d[_i][_j], d2[_i][_j], dv[_i][_j]);
                d2[_j][_i] = d2[_i][_j];
                dv[_i][_j].negate(dv[_j][_i]);
            }
        }
    }
}

}
