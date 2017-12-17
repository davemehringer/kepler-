//# PairWiseAccelerationCalculator.cc
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
 * PairWiseAccelerationCalculator.cc
 *
 *  Created on: Sep 18, 2015
 *      Author: dmehring
 */

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>

#include <Physics/Body.h>
#include <BodyFrameRotMatrix/BodyFrameRotMatrix.h>
#include <DistanceCalculator/DistanceCalculator.h>
#include <Math/RotMatrix3x3.h>
#include <IO/IO.h>

using namespace std;

namespace kepler {

PairWiseAccelerationCalculator::PairWiseAccelerationCalculator(
        DistanceCalculator* dc, const vector<Body> *const  &bodies
) : AccelerationCalculator(dc), _bodies(bodies),
    _d(bodies->size(), vector<PrecType>(bodies->size())),
    _d2(bodies->size(), vector<PrecType>(bodies->size())),
    _dv(bodies->size(), vector<Vector>(bodies->size())),
    _diff(bodies->size(), vector<Vector>(bodies->size())),
    _hasJ(bodies->size()),
    _jRadLimit(bodies->size(), 0) {
    auto hj = begin(_hasJ);
    auto jrl = begin(_jRadLimit);
    for (const auto& b: *_bodies) {
        *hj = (bool)b.j;
        if (*hj) {
            *jrl = 100*b.radius*100*b.radius;
        }
        ++hj;
        ++jrl;
    };
}

PairWiseAccelerationCalculator::~PairWiseAccelerationCalculator() {}

void PairWiseAccelerationCalculator::compute(
        Vvector& res, const Vvector *const  &x
) {
    _dc->compute(_d, _d2, _dv, _diff, *x);
    _compute(res, x);
}

void PairWiseAccelerationCalculator::_compute(Vvector& res, const Vvector *const  &) {
    int i = 0;
    int j = 0;
    for (const auto& body0: *_bodies) {
        res[i] = 0;
        j = 0;
        for (const auto& body1: *_bodies) {
            if (i != j && body1.mu != 0) {
                // accumulate negative values because gravity is
                // an attractive force
                _dv[i][j].times(_scratch, body1.mu/_d2[i][j]);
                res[i] -= _scratch;
                if (_hasJ[j] && body0.centerBody && body0.centerBody->id == body1.id && _d2[i][j] < _jRadLimit[j]) {
                    if (i > j) {
                        _d[i][j] = _d[j][i];
                        _diff[j][i].negate(_diff[i][j]);
                    }
                    _doJContrib(_scratch, _d[i][j], _d2[i][j], _diff[i][j], body1);
                    res[i] += _scratch;
                }
            }
            ++j;
        }
        ++i;
    }
}

}
