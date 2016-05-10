//# tSSObjects.cc
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
#include <SolarSystem/SSObjects.h>

#include <Exception/KeplerException.h>

using namespace kepler;

TEST(SSObjectsTest, jplID) {
	ASSERT_EQ(502, SSObjects::jplID("europa"));
	ASSERT_THROW(SSObjects::jplID("bozo"), KeplerException);
}

/*
TEST(SSObjectsTest, name) {
	ASSERT_STREQ("europa", SSObjects::name(502).c_str());
	ASSERT_THROW(SSObjects::name(10000), invalid_argument);
}

TEST(SSObjectsTest, mu) {
	ASSERT_PrecType_EQ(4902.798, SSObjects::mu(301));
	ASSERT_THROW(SSObjects::mu(10000), invalid_argument);
}

TEST(SSObjectsTest, radius) {
	ASSERT_PrecType_EQ(71492, SSObjects::radius(599));
	ASSERT_THROW(SSObjects::radius(10000), invalid_argument);
}

TEST(SSObjectsTest, j) {
	vector<PrecType> j = SSObjects::j(599);
	ASSERT_EQ(7, j.size());
	ASSERT_PrecType_EQ(1.4695625e-2, j[2]);
	ASSERT_THROW(SSObjects::j(10000), invalid_argument);
}
*/

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

