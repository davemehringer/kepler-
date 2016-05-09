/*
 * tBody.cc
 *
 *  Created on: Aug 30, 2015
 *      Author: dmehring
 */

#include <gtest/gtest.h>
#include <Physics/Body.h>

using namespace kepler;

TEST(BodyTest, Constructor) {
    /*
	Vector x(1, 2, 3);
	Vector v(4, 5, 6);
	Body b("Jupiter", 4, x, v);
	ASSERT_STREQ("Jupiter", b.getName().c_str());
	ASSERT_EQ(4, b.getMu());
	ASSERT_EQ(x, b.getX());
	ASSERT_EQ(v, b.getV());
	*/
}
int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

