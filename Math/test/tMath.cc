//# tMath.cc
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


