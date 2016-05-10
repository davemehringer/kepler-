//# tBody.cc
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

