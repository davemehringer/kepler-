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

void PairWiseAccelerationCalculator::_compute(Vvector& res, const Vvector *const  &x) {
    int i = 0;
    int j = 0;
    for (const auto& body0: *_bodies) {
        res[i] = 0;
        j = 0;
        for (const auto& body1: *_bodies) {
            if (i != j) {
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

/*
// subclasses can use threads to call this method, so don't use
// writable private fields in it
void PairWiseAccelerationCalculator::_doJContrib(
    Vector& aj, PrecType d, PrecType d2, const Vector& diffEc, const Body& body
)  {
    auto matrix = body.bfrm->ecToEq(_time, SECOND);
    auto diffEq = diffEc*matrix;
    auto x = diffEq[0];
    auto y = diffEq[1];
    auto z = diffEq[2];
    auto z2 = z*z;
    auto ff = -d2 + 5*z2;
    aj[0] = x*ff;
    aj[1] = y*ff;
    aj[2] = z*(-3*d2 + 5*z2);
    auto d7 = d2*d2*d2*d;
    aj *= body.c2/d7;
    if (
        body.j->size() >= 5
        && (body.j->operator[](4) != 0 || body.j->operator[](6) != 0 || body.j->operator[](8) != 0)
    ) {
            auto d4 = d2*d2;
            auto z4 = z2*z2;
            auto d11 = d4*d7;
            auto j4 = body.j->operator[](4);
            if (j4 != 0) {
                auto xy = 3*(d4 - 14*d2*z2 + 21*z4);
                Vector aj4 {
                    x*xy, y*xy, z*(15*d4 + -70*d2*z2 + 63*z4)
                };
                aj4 *= body.c4/d11;
                aj += aj4;
            }
            if(
                body.j->size() >= 7 && (body.j->operator[](6) != 0 || body.j->operator[](8) != 0)
            ) {
                auto z6 = z2*z4;
                auto x2 = x*x;
                auto x4 = x2*x2;
                auto x6 = x2*x4;
                auto y2 = y*y;
                auto y4 = y2*y2;
                auto y6 = y2*y4;
                auto d15 = d11*d4;
                auto j6 = body.j->operator[](6);
                if (j6 != 0 && d < 50*body.radius) {
                    auto xy = 5*x6 + 15*x4*y2 - 120*x4*z2 + 15*x2*y4 - 240*x2*y2*z2 + 240*x2*z4 + 5*y6 - 120*y4*z2 + 240*y2*z4 - 64*z6;
                    Vector aj6 {x*xy, y*xy, 0};
                    aj6[2] = z*(35*x6 + 105*x4*y2 - 210*x4*z2 + 105*x2*y4 - 420*x2*y2*z2 + 168*x2*z4 + 35*y6 - 210*y4*z2 + 168*y2*z4 - 16*z6);
                    aj6 *= body.c6/d15;
                    aj += aj6;
                }
                if (body.j->size() >= 9 && body.j->operator[](8) != 0 &&  d < 30*body.radius) {
                    auto z8 = z4*z4;
                    auto x8 = x4*x4;
                    auto y8 = y4*y4;
                    auto d19 = d15*d4;
                    auto xy = 5.0*(7*x8 + 28*x6*y2 - 280*x6*z2 + 42*x4*y4 - 840*x4*y2*z2
                        + 1120*x4*z4 + 28*x2*y6 - 840*x2*y4*z2 + 2240*x2*y2*z4 - 896*x2*z6
                        + 7*y8 - 280*y6*z2 + 1120*y4*z4 - 896*y2*z6 + 128*z8);
                    Vector aj8 {x*xy, y*xy, 0};
                    aj8[2] = z*(
                        315*x8 + 1260*x6*y2 - 3360*x6*z2 + 1890*x4*y4 - 10080*x4*y2*z2
                        + 6048*x4*z4 + 1260*x2*y6 - 10080*x2*y4*z2 + 12096*x2*y2*z4
                        - 2304*x2*z6 + 315*y8 - 3360*y6*z2 + 6048*y4*z4 - 2304*y2*z6 + 128*z8
                    );
                    aj8 *= body.c8/d19;
                    aj += aj8;
                }
            }
    }
    aj = aj * body.bfrm->eqToEc(_time, SECOND);
}
*/
/*


    body.
    if self._is_ecliptic:
        r = numpy.dot(self._eclip_to_body[body_number],s)
    else:
        r = s
    [x,y,z] = r
    z2 = z*z
    d2 = d*d
    ff = -d2 + 5*z2
    aj = numpy.array([0.0,0.0,0.0])
    aj[0] = x*ff
    aj[1] = y*ff
    aj[2] = z*(-3*d2 + 5*z2)
    R = self._radius[body_number]
    R2 = R*R
    d7 = d**7
    mu = self.m[body_number]
    J = self.J[body_number]
    aj *= 1.5*J[2]*mu*R2
    aj /= d7
    if len(J) >= 5 and (J[4] != 0 or J[6] != 0 or J[8] != 0):
        d4 = d2*d2
        z4 = z2*z2
        R4 = R2*R2
        d11 = d4*d7
    if len(J) >= 5 and J[4] != 0 and d < 100*R:
        aj4 = numpy.array([0.0, 0.0, 0.0])
        xy = 3*(d4 - 14*d2*z2 + 21*z4)
        aj4[0] = x*xy
        aj4[1] = y*xy
        aj4[2] = z*(15*d4 + -70*d2*z2 + 63*z4)
        aj4 *= 5.0/8.0*J[4]*R4*mu/d11
        aj += aj4
    if len(J) >= 7 and J[6] != 0 or J[8] != 0:
        z6 = z2*z4
        x2 = x*x
        x4 = x2*x2
        x6 = x2*x4
        y2 = y*y
        y4 = y2*y2
        y6 = y2*y4
        R6 = R2*R4
        d15 = d11*d4
    if len(J) >= 7 and J[6] != 0 and d < 30*R:
        aj6 = numpy.array([0.0,0.0,0.0])
        xy = 5*x6 + 15*x4*y2 - 120*x4*z2 + 15*x2*y4 - 240*x2*y2*z2 + 240*x2*z4 + 5*y6 - 120*y4*z2 + 240*y2*z4 - 64*z6
        aj6[0] = x*xy
        aj6[1] = y*xy
        aj6[2] = z*(35*x6 + 105*x4*y2 - 210*x4*z2 + 105*x2*y4 - 420*x2*y2*z2 + 168*x2*z4 + 35*y6 - 210*y4*z2 + 168*y2*z4 - 16*z6)
        aj6 *= -7.0/16.0*J[6]*R6*mu/d15
        aj += aj6
    if len(J) >= 9 and J[8] != 0 and d < 10*self._radius[body_number]:
        z8 = z4*z4
        x8 = x4*x4
        y8 = y4*y4
        R8 = R6*R2
        d19 = d15*d4
        aj8 = numpy.array([0.0, 0.0, 0.0])
        xy = 5.0*(7*x8 + 28*x6*y2 - 280*x6*z2 + 42*x4*y4 - 840*x4*y2*z2 + 1120*x4*z4 + 28*x2*y6 - 840*x2*y4*z2 + 2240*x2*y2*z4 - 896*x2*z6 + 7*y8 - 280*y6*z2 + 1120*y4*z4 - 896*y2*z6 + 128*z8)
        aj8[0] = x*xy
        aj8[1] = y*xy
        aj8[2] = z*(315*x8 + 1260*x6*y2 - 3360*x6*z2 + 1890*x4*y4 - 10080*x4*y2*z2 + 6048*x4*z4 + 1260*x2*y6 - 10080*x2*y4*z2 + 12096*x2*y2*z4 - 2304*x2*z6 + 315*y8 - 3360*y6*z2 + 6048*y4*z4 - 2304*y2*z6 + 128*z8)
        aj8 *= 9.0/128.0*J[8]*R8*mu/d19
        aj += aj8

    if self._is_ecliptic:
        aj = numpy.dot(self._body_to_eclip[body_number], aj)
    return aj

}
*/

}
