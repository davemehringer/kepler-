//# tSerialTargetedDistanceCalculator.cc
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
 * tVector.cc
 *
 *  Created on: Aug 29, 2015
 *      Author: dmehring
 */

#include <gtest/gtest.h>
#include <DistanceCalculator/SerialTargetedDistanceCalculator.h>

#include <Math/Math.h>
#include <Math/Vector.h>

using namespace kepler;

TEST(SerialTargetedDistanceCalculator, compute) {
    SerialTargetedDistanceCalculator stdc;
    Vvector v(100);
    int i = 0;
    for (auto& x: v) {
        x = Vector(i, i, i);
        ++i;
    }
    DMatrix d(100, vector<PrecType>(100));
    DMatrix d2(100, vector<PrecType>(100));
    VMatrix dv(100, Vvector(100));
    VMatrix diff(100, Vvector(100));
    set<int> targets;
    targets.insert(10);
    targets.insert(20);
    stdc.setTargets(&targets);
    stdc.compute(d, d2, dv, diff, v);
    auto sqrt3 = sqrt(3);
    Vector expdv(1/sqrt3, 1/sqrt3, 1/sqrt3);
    for (int i=0; i<100; ++i) {
        for (int j=0; j<100; ++j) {
            if (i == j || (i != 10 && i != 20)) {
                ASSERT_EQ(0, d2[i][j]);
                ASSERT_EQ(0, d[i][j]);
                ASSERT_EQ(Vector(0, 0, 0), dv[i][j]);
                ASSERT_EQ(Vector(0, 0, 0), diff[i][j]);
            }
            else {
                ASSERT_TRUE(i == 10 || i == 20);
                auto k = i-j;
                ASSERT_EQ(3*k*k, d2[i][j]);
                ASSERT_TRUE(near( i > j ? expdv : expdv.negate(), dv[i][j], 1e-8, 1e-8));
                ASSERT_TRUE(near(sqrt3*abs(k), d[i][j], 1e-10));
                ASSERT_EQ(Vector(k, k, k), diff[i][j]);
            }
        }
    }
}


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


