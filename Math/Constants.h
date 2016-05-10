//# Constants.h
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
 * Constants.h
 *
 *  Created on: Aug 30, 2015
 *      Author: dmehring
 */

#ifndef SRC_CONSTANTS_H_
#define SRC_CONSTANTS_H_

#include <Math/Types.h>

#include <math.h>

namespace kepler {

// Physical constants

enum TimeUnit {
    SECOND,
    DAY
};

// gravitation constant in units of km, s, kg

// current NIST value of G
static const PrecType G_NIST = 6.67408e-20; // +/- 0.00031e-20

// G = mu/m
// from JPL solar values of mu = 1.3271244004193938e11
// m = 1.988544e30
static const PrecType G_JPL_SUN = 6.673850e-20;

// from JPL Jupiter values of mu = 126686511
// and m=1898.13e24+/-0.19e24
static const PrecType G_JPL_JUP = 6.67428e-20; // +/- 0.00010e-20

// from JPL Saturn values of mu = 37931207.8 +/- 98
// and m = 5.68319e26
static const PrecType G_JPL_SAT = 6.674281e-20; // +/- 000017e-20

// from JPL Earth values of mu = 398600.440
// and m = 5.97219e24 +/- 0.0006e24

static const PrecType G_JPL_EARTH = 6.67428e-20; // +/- 0.00067e-20

// we currently adopt the NIST value

static const PrecType G = G_NIST;

static const auto RAD_PER_DEG = M_PI/180.0;

static const auto DEG_PER_RAD= 1/RAD_PER_DEG;

static const PrecType KM_PER_AU = 149597870.700;

static const PrecType SECOND_PER_DAY = 86400;

static const auto KMPERSEC_PER_AUPERDAY = KM_PER_AU/SECOND_PER_DAY;

}


#endif /* SRC_CONSTANTS_H_ */
