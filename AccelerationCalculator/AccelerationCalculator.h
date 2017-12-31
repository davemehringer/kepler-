//# AccelerationCalculator.h
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
 * AccelerationCalculator.h
 *
 *  Created on: Sep 18, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_ACCELERATIONCALCULATOR_H_
#define KEPLER_ACCELERATIONCALCULATOR_H_

#include <BodyFrameRotMatrix/BodyFrameRotMatrix.h>
#include <IO/IO.h>
#include <Math/Constants.h>
#include <Math/RotMatrix3x3.h>
#include <Math/Types.h>
#include <Physics/Body.h>

#include <iomanip>
#include <vector>

using namespace std;

namespace kepler {

class DistanceCalculator;

class AccelerationCalculator {
public:

    virtual ~AccelerationCalculator() {}

    virtual void compute(Vvector& res, const Vvector *const &x) = 0;

    inline void setTime(PrecType t, TimeUnit unit) { _time = unit == DAY ? SECOND_PER_DAY*t : t; }

    // subclasses can use threads to call this method, so don't use
    // writable private fields in it
    inline void doJContrib(
        Vector& aj, PrecType d, PrecType d2, const Vector& diffEc, const Body& body
    ) const {
        const auto& diffEq = diffEc*body.bfrm->ecToEq(_time, SECOND);
        const auto& x = diffEq[0];
        const auto& y = diffEq[1];
        const auto& z = diffEq[2];
        // J2 term
        auto r = d/body.radius;
        auto r2 = r*r;
        auto d3 = d2*d;
        auto cr = body.c[2]/r2;
        auto z_d = z/d;
        auto z_d2 = z_d * z_d;
        auto _2_5_z_d2 = 2.5*z_d2;
        auto ff = cr*(-0.5 + _2_5_z_d2)/d3;
        aj[0] = x*ff;
        aj[1] = y*ff;
        aj[2] = cr*z*(-1.5 + _2_5_z_d2)/d3;
        auto nj = body.j->size();
        auto doJ3  = nj >=  4 && body.j->operator[](3)  != 0;
        auto doJ4  = nj >=  5 && body.j->operator[](4)  != 0;
        auto doJ5  = nj >=  6 && body.j->operator[](5)  != 0;
        auto doJ6  = nj >=  7 && body.j->operator[](6)  != 0;
        auto doJ7  = nj >=  8 && body.j->operator[](7)  != 0;
        auto doJ8  = nj >=  9 && body.j->operator[](8)  != 0;
        auto doJ9  = nj >= 10 && body.j->operator[](9)  != 0;
        auto doJ10 = nj >= 11 && body.j->operator[](10) != 0;
        if (
            doJ3 || doJ4 || doJ5 || doJ6 || doJ7
            || doJ8 || doJ9 || doJ10
        ) {
            auto d4 = d2*d2;
            auto z_d4 = z_d2 * z_d2;
            if (doJ3) {
                auto r3 = r2*r;
                cr = body.c[3]/r3;
                ff = 10*z*cr*(-1.5 + 3.5*z_d2)/d4;
                Vector aj3 {
                    x*ff, y*ff,
                    cr*(3.0 - 30.0*z_d2 + 35.0*z_d4)/d2
                };
                aj += aj3;
            }
            if (
                doJ4 || doJ5 || doJ6 || doJ7
                || doJ8 || doJ9 || doJ10
            ) {
                auto r4 = r2*r2;
                if (doJ4) {
                    cr = body.c[4]/r4;
                    ff = 6.0*cr*(0.125 - 1.75*z_d2 + 2.625*z_d4)/d3;
                    Vector aj4 {
                        ff*x, ff*y,
                        cr*z*(3.75 - 17.5*z_d2 + 15.75*z_d4)/d3
                    };
                    aj += aj4;
                }
                if (doJ5 || doJ6 || doJ7 || doJ8 || doJ9 || doJ10) {
                    auto z_d6 = z_d2 * z_d4;
                    if (doJ5) {
                        auto r5 = r4*r;
                        cr = body.c[5]/r5;
                        ff = 56.0*cr*(0.625 - 3.75*z_d2 + 4.125*z_d4)/d4;
                        Vector aj5 {
                            ff*x*z, ff*y*z,
                            cr*(-5.0 + 105.0*z_d2 - 315.0*z_d4 + 231.0*z_d6)/d2
                        };
                        aj += aj5;
                    }
                    if (doJ6 || doJ7 || doJ8 || doJ9 || doJ10) {
                        auto r6 = r4*r2;
                        if (doJ6) {
                            cr = body.c[6]/r6;
                            ff = 8.0*cr*(
                                -0.3125 + 8.4375*z_d2 - 30.9375*z_d4 + 26.8125*z_d6
                            )/d3;
                            Vector aj6 = {
                                ff*x, ff*y,
                                cr*z*(
                                    -17.5 + 157.5*z_d2 - 346.5*z_d4 + 214.5*z_d6
                                )/d3
                            };
                            aj += aj6;
                        }
                        if (doJ7 || doJ8 || doJ9 || doJ10) {
                            auto z_d8 = z_d4 * z_d4;
                            if (doJ7) {
                                auto r7 = r6*r;
                                cr = body.c[7]/r7;
                                ff = 144*cr*(
                                    -2.1875 + 24.0625*z_d2 - 62.5625*z_d4
                                    + 44.6875*z_d6
                                )/d4;
                                Vector aj7 {
                                    x*z*ff,
                                    y*z*ff,
                                    cr*(
                                        35.0 - 1260.0*z_d2 + 6930.0*z_d4
                                        - 12012.0*z_d6 + 6435.0*z_d8
                                    )/d2
                                };
                                aj += aj7;
                            }
                            if (doJ8 || doJ9 || doJ10) {
                                auto r8 = r6*r2;
                                if (doJ8) {
                                    cr = body.c[8]/r8;
                                    ff = 80*cr*(
                                        0.0546875 - 2.40625*z_d2 + 15.640625*z_d4
                                        - 31.28125*z_d6 + 18.9921875*z_d8
                                    )/d3;
                                    Vector aj8 {
                                        ff*x, ff*y,
                                        cr*z*(
                                             39.375 - 577.5*z_d2 + 2252.25*z_d4
                                             - 3217.5*z_d6 + 1519.375*z_d8
                                        )/d3
                                    };
                                    aj += aj8;
                                }
                                if (doJ9 || doJ10) {
                                    auto z_d10 = z_d6 * z_d4;
                                    if (doJ9) {
                                        auto r9 = r8*r;
                                        cr = body.c[9]/r9;
                                        ff = 1408*cr*(
                                             0.4921875 - 8.53125*z_d2
                                             + 38.390625*z_d4 - 62.15625*z_d6
                                             + 32.8046875*z_d8
                                        )/d4;
                                        Vector aj9 {
                                            x*z*ff, y*z*ff,
                                            cr*(
                                                 -63.0 + 3465.0*z_d2 - 30030.0*z_d4
                                                 + 90090.0*z_d6 - 109395.0*z_d8
                                                 + 46189.0*z_d10
                                            )/d2
                                        };
                                        aj += aj9;
                                    }
                                    if (doJ10) {
                                        auto r10 = r8*r2;
                                        cr = body.c[10]/r10;
                                        ff = 384*cr*(
                                             -0.08203125 + 5.33203125*z_d2
                                             - 53.3203125*z_d4 + 181.2890625*z_d6
                                             - 246.03515625*z_d8
                                             + 114.81640625*z_d10
                                        )/d3;
                                        Vector aj10 {
                                            x*ff, y*ff,
                                            cr*z*(
                                                -346.5 + 7507.5*z_d2 - 45045.0*z_d4
                                                + 109395.0*z_d6 - 115472.5*z_d8
                                                + 44089.5*z_d10
                                            )/d3
                                        };
                                        aj += aj10;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        aj = aj * body.bfrm->eqToEc(_time, SECOND);
    }

protected:

    DistanceCalculator *_dc;
    // system time, for things like ecliptic to equatorial rotation matrices which
    // are time dependent
    PrecType _time = 0;

    AccelerationCalculator(DistanceCalculator* dc) : _dc(dc) {}
};

}

#endif
