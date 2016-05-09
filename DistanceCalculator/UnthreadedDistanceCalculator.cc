/*
 * UnthreadedDistanceCalculator.cc
 *
 *  Created on: Sep 29, 2015
 *      Author: dmehring
 */

#include <DistanceCalculator/UnthreadedDistanceCalculator.h>

namespace kepler {
Timer UnthreadedDistanceCalculator::_timer;
UnthreadedDistanceCalculator::UnthreadedDistanceCalculator() {}

UnthreadedDistanceCalculator::~UnthreadedDistanceCalculator() {}

void UnthreadedDistanceCalculator::compute(
    DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const Vvector& x
) {
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

}
