/* *
 *      Author: dmehring
 */

#include <DistanceCalculator/SerialTargetedDistanceCalculator.h>

#include <Math/Vector.h>

namespace kepler {

Timer SerialTargetedDistanceCalculator::_timer;
SerialTargetedDistanceCalculator::SerialTargetedDistanceCalculator() {}

SerialTargetedDistanceCalculator::~SerialTargetedDistanceCalculator() {}

void SerialTargetedDistanceCalculator::compute(
    DMatrix& d, DMatrix& d2, VMatrix& dv, VMatrix& diff, const Vvector& x
) {
    _titer = begin(*_targets);
    _tend = end(*_targets);
    // the first set of loops does the necessary pair calculations
    for(; _titer != _tend; ++_titer) {
        _i = *_titer;
        _viter = _titer;
        ++_viter;
        for (; _viter!=_tend; ++_viter) {
            _j = *_viter;
            x[_i].subtract(diff[_i][_j], x[_j]);
            diff[_i][_j].negate(diff[_j][_i]);
            diff[_i][_j].magAndmag2AndUnit(d[_i][_j], d2[_i][_j], dv[_i][_j]);
            d[_j][_i] = d[_i][_j];
            d2[_j][_i] = d2[_i][_j];
            dv[_i][_j].negate(dv[_j][_i]);
        }
    }
    _xend = end(x);
    for (auto t: *_targets) {
        _j = 0;
        _yiter = begin(x);
        _titer = begin(*_targets);
        for ( ; _yiter != _xend; ++_yiter, ++_j) {
            if (_j == *_titer) {
                // this distance pair has been calculated in the
                // previous nested loop
                ++_titer;
            }
            else {
                x[t].subtract(diff[t][_j], *_yiter);
                diff[t][_j].magAndmag2AndUnit(d[t][_j], d2[t][_j], dv[t][_j]);
            }
        }
    }
}

}
