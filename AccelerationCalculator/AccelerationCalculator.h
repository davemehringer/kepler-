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

protected:

    DistanceCalculator *_dc;
    // system time, for things like ecliptic to equatorial rotation matrices which
    // are time dependent
    PrecType _time = 0;

    AccelerationCalculator(DistanceCalculator* dc) : _dc(dc) {}

    // subclasses can use threads to call this method, so don't use
    // writable private fields in it
    inline void _doJContrib(
        Vector& aj, PrecType d, PrecType d2, const Vector& diffEc, const Body& body
    ) const {
        /*
        aj[0] = 0;
        aj[1] = 0;
        aj[2] = 0;
        */
        auto diffEq = diffEc*body.bfrm->ecToEq(_time, SECOND);
        auto x = diffEq[0];
        auto y = diffEq[1];
        auto z = diffEq[2];

        auto z2 = z*z;
        auto dd2 = d*d;
        // J2 term
        auto r = d/body.radius;
        auto r2 = r*r;
        auto d5 = d2*d2*d;
        auto cr = body.c[2]/r2;
        auto ff = cr*(-0.5*dd2 + 2.5*z2)/d5;
        aj[0] = x*ff;
        aj[1] = y*ff;
        aj[2] = cr*z*(-1.5*dd2 + 2.5*z2)/d5;
        auto nj = body.j->size();

        auto doJ3 = nj >= 4 && body.j->operator[](3) != 0;
        auto doJ4 = nj >= 5 && body.j->operator[](4)  != 0;
        auto doJ5 = nj >= 6 && body.j->operator[](5)  != 0;
        auto doJ6 = nj >= 7 && body.j->operator[](6)  != 0;
        auto doJ7 = nj >= 8 && body.j->operator[](7)  != 0;
        auto doJ8 = nj >= 8 && body.j->operator[](8)  != 0;

        if (doJ3 || doJ4 || doJ5 || doJ6 || doJ7 || doJ8) {
            Vector aj3;
            auto z4 = z2*z2;
            auto d4 = d2*d2;
            auto d6 = d4*d2;
            if (doJ3) {
                auto r3 = r2*r;
                cr = body.c[3]/r3;
                ff = 10*z*cr*(-1.5*d2 + 3.5*z2)/d6;
                Vector aj3 {
                    x*ff, y*ff,
                    cr*(3.0*d4 - 30.0*d2*z2 + 35.0*z4)/d6
                };
                aj += aj3;
            }
            if (doJ4 || doJ5 || doJ6 || doJ7 || doJ8) {
                auto r4 = r2*r2;
                auto d2z2 = d2*z2;
                if (doJ4) {
                    auto d7 = d6*d;
                    cr = body.c[4]/r4;
                    ff = 6.0*cr*(0.125*d4 - 1.75*d2z2 + 2.625*z4)/d7;
                    Vector aj4 {
                        ff*x, ff*y,
                        cr*z*(3.75*d4 - 17.5*d2z2 + 15.75*z4)/d7
                    };
                    aj += aj4;
                }
                if (doJ5 || doJ6 || doJ7 || doJ8) {
                    auto d8 = d4*d4;
                    auto d4z2 = d4*z2;
                    auto d2z4 = d2*z4;
                    if (doJ5) {
                        auto z6 = z2*z2*z2;
                        auto r5 = r4*r;
                        cr = body.c[5]/r5;
                        ff = 56.0*cr*(0.625*d4 - 3.75*d2z2 + 4.125*z4)/d8;
                        Vector aj5 {
                            ff*x*z, ff*y*z,
                            cr*(-5.0*d6 + 105.0*d4z2 - 315.0*d2z4 + 231.0*z6)/d8
                        };
                        aj += aj5;
                    }
                    if (doJ6 || doJ7 || doJ8) {
                        auto r6 = r4*r2;
                        if (doJ6) {
                            auto z6 = z4*z2;
                            auto d9 = d8*d;
                            cr = body.c[6]/r6;
                            ff = 8.0*cr*(-0.3125*d6 + 8.4375*d4z2 - 30.9375*d2z4 + 26.8125*z6)/d9;
                            Vector aj6 = {
                                ff*x, ff*y,
                                cr*z*(-17.5*d6 + 157.5*d4z2 - 346.5*d2z4 + 214.5*z6)/d9
                            };
                            aj += aj6;
                        }
                        if (doJ7 || doJ8) {
                            auto d10 = d6*d4;
                            auto z6 = z4*z2;
                            auto z8 = z6*z2;
                            auto d6z2 = d6*z2;
                            auto d2z6 = d2*z6;
                            auto d4z4 = d4*z4;
                            if (doJ7) {
                                auto r7 = r6*r;
                                cr = body.c[7]/r7;
                                ff = 144*cr*(-2.1875*d6 + 24.0625*d4z2 - 62.5625*d2z4 + 44.6875*z6)/d10;
                                Vector aj7 {
                                    x*z*ff,
                                    y*z*ff,
                                    cr*(35.0*d8 - 1260.0*d6z2 + 6930.0*d4z4 - 12012.0*d2z6 + 6435.0*z8)/d10
                                };
                                aj += aj7;
                            }
                            if (doJ8) {
                                auto d11 = d10*d;
                                auto r8 = r6*r2;
                                cr = body.c[8]/r8;
                                ff = 80*cr*(0.0546875*d8 - 2.40625*d6z2 + 15.640625*d4z4 - 31.28125*d2z6 + 18.9921875*z8)/d11;
                                Vector aj8 {
                                    ff*x, ff*y,
                                    cr*z*(39.375*d8 - 577.5*d6z2 + 2252.25*d4z4 - 3217.5*d2z6 + 1519.375*z8)/d11
                                };
                                aj += aj8;
                            }
                        }

                    }
                }
            }

        }
        aj = aj * body.bfrm->eqToEc(_time, SECOND);
    }

};

}

#endif
