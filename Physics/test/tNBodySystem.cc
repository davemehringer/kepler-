//# tNBodySystem.cc
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
#include <Physics/NBodySystem.h>

using namespace kepler;

TEST(NBodySystemTest, Constructors) {
	Body b0;
	b0.mu = 4;
	b0.x = Vector(10, 15, 20);
	b0.v = Vector(-10, -15, -20);
	Body b1;
	b1.mu = 1;
    b1.x = Vector(5, 10, 15);
    b1.v = Vector(-5, -10, -15);
    vector<Body> bodies {b0, b1};
    NBodySystem system(bodies, 0, DAY);
    const auto& myb = system.getBodies();
    ASSERT_EQ(Vector(1, 1, 1), myb[0].x);
    ASSERT_EQ(Vector(-4, -4, -4), myb[1].x);
    ASSERT_EQ(Vector(-1, -1, -1), myb[0].v);
    ASSERT_EQ(Vector(4, 4, 4), myb[1].v);
}

TEST(NBodySystemTest, step) {
    Body b0;
    b0.mu = 4;
    b0.x = Vector(10, 15, 20);
    b0.v = Vector(-10, -15, -20);
    Body b1;
    b1.mu = 1;
    b1.x = Vector(5, 10, 15);
    b1.v = Vector(-5, -10, -15);
    vector<Body> bodies {b0, b1};
    NBodySystem system(bodies, 0, DAY);
    Vector x0(6, 7, 8);
    Vector x1(10, 12, 20);
    Vector v0(-6, -7, -8);
    Vector v1(-10, -12, -20);
    vector<Vector> x {x0, x1};
    vector<Vector> v {v0, v1};
    system.step(x, v, 5, DAY);
    const auto& myb = system.getBodies();
    ASSERT_EQ(Vector(7, 8, 9), myb[0].x);
    ASSERT_EQ(Vector(6, 8, 16), myb[1].x);
    ASSERT_EQ(Vector(-7, -8, -9), myb[0].v);
    ASSERT_EQ(Vector(-6, -8, -16), myb[1].v);
    ASSERT_EQ(5, system.getTime(DAY));
}

TEST(NBodySystemTest, getEnergy) {
    Body b0;
    b0.mu = 4;
    b0.x = Vector(10, 15, 20);
    b0.v = Vector(-10, -15, -20);
    Body b1;
    b1.mu = 1;
    b1.x = Vector(5, 10, 15);
    b1.v = Vector(-5, -10, -15);
    Body b2;
    b2.mu = 10;
    b2.x = Vector(7, 6, 5);
    b2.v = Vector(-7, -6, -5);
    vector<Body> bodies {b0, b1, b2};
    NBodySystem system(bodies, 0, DAY);
    auto energy = system.getEnergy();
    ASSERT_DOUBLE_EQ(466.37150417619699, energy);
}

TEST(NBodySystemTest, getAngularMomentum) {
    Body b0;
    b0.mu = 4;
    b0.x = Vector(10, 15, 20);
    b0.v = Vector(-10, -15, -20);
    Body b1;
    b1.mu = 1;
    b1.x = Vector(5, 10, 15);
    b1.v = Vector(-5, -10, -15);
    Body b2;
    b2.mu = 10;
    b2.x = Vector(7, 6, 5);
    b2.v = Vector(3, 4, 5);
    vector<Body> bodies {b0, b1, b2};
    NBodySystem system(bodies, 0, DAY);
    auto angmom = system.getAngularMomentum();
    cout << "L " << angmom << endl;
    ASSERT_TRUE(near(Vector(200, -400, 200), angmom,1e-18, 1e-7));

}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


