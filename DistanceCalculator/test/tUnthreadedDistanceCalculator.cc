/*
 * tVector.cc
 *
 *  Created on: Aug 29, 2015
 *      Author: dmehring
 */

#include <gtest/gtest.h>
#include <DistanceCalculator/UnthreadedDistanceCalculator.h>

#include <Math/Math.h>
#include <Math/Vector.h>

using namespace kepler;

TEST(UnthreadedDistanceCalculator, compute) {
    UnthreadedDistanceCalculator udc;
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
    udc.compute(d, d2, dv, diff, v);
    auto sqrt3 = sqrt(3);
    Vector expdv(1/sqrt3, 1/sqrt3, 1/sqrt3);
    for (int i=0; i<100; ++i) {
        for (int j=0; j<100; ++j) {
            if (i == j) {
                ASSERT_EQ(0, d2[i][j]);
                ASSERT_EQ(0, d[i][j]);
                ASSERT_EQ(Vector(0, 0, 0), dv[i][j]);
                ASSERT_EQ(Vector(0, 0, 0), diff[i][j]);
            }
            else {
                auto k = i-j;
                ASSERT_EQ(3*k*k, d2[i][j]);
                ASSERT_TRUE(near( i > j ? expdv : expdv.negate(), dv[i][j], 1e-8, 1e-8));
                if (j > i) {
                    ASSERT_TRUE(near(sqrt3*abs(k), d[i][j], 1e-10));
                    ASSERT_EQ(Vector(k, k, k), diff[i][j]);
                }
                else {
                    ASSERT_EQ(0, d[i][j]);
                    ASSERT_EQ(Vector(0, 0, 0), diff[i][j]);
                }
            }
        }
    }
}


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


