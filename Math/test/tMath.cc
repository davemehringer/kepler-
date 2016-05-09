/*
 * tMath.cc
 *
 *      Author: dmehring
 */

#include <gtest/gtest.h>
#include <Math/Math.h>

using namespace kepler;

TEST(MathTest, near) {
	ASSERT_TRUE(near(11, 11, 1e-8));
	ASSERT_TRUE(near(0.09, 0, 1e-1));
	ASSERT_FALSE(near(0.09, 0, 1e-2));
	ASSERT_TRUE(near(11, 12, 1e-1));
	ASSERT_FALSE(near(11, 12, 1e-2));
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


