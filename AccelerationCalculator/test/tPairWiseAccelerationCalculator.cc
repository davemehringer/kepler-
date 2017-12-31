//# tPairWiseAccelerationCalculator.cc
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
#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>

#include <Physics/Body.h>
#include <DistanceCalculator/UnthreadedDistanceCalculator.h>

using namespace kepler;

TEST(PairWiseAccelerationCalculator, compute) {
	Body body0;
	body0.mu = 1;
	Body body1;
	body1.mu = 2;
	Body body2;
	body2.mu = 3;
	vector<Body> bodies {body0, body1, body2};

	Vector x0 = {0, -1, 2};
	Vector x1 = {-3, 4, -5};
	Vector x2 = {6, -7, 8};

	Vvector x {x0, x1, x2};
	Vvector a(bodies.size());

	UnthreadedDistanceCalculator udc;
	PairWiseAccelerationCalculator pwac(&udc, &bodies);
	pwac.compute(a, &x);
	Vector expec {
	    0.0081027416951023491,
	    -0.0028128978401775126,
	    -0.0024769460147473221
	};
	cout << expec << endl;
	cout << a[0] << endl;
	//ASSERT_TRUE(near(expec, a[0], 1e-8, 1e-8));
    ASSERT_DOUBLE_EQ(expec[0], a[0][0]);
    ASSERT_DOUBLE_EQ(expec[1], a[0][1]);
    ASSERT_DOUBLE_EQ(expec[2], a[0][2]);
	expec = Vector(
	    0.007745739105366526,
	    -0.011230295747089588,
	    0.014714852388812649
	);
	cout << expec << endl;
	cout << a[1] << endl;
    ASSERT_TRUE(near(expec, a[1], 1e-12, 1e-12));

    expec = Vector(
        -0.0078647399686118009, 0.0084244964447855626,
        -0.008984252920959326
    );
    //ASSERT_TRUE(near(expec, a[2], 1e-12, 1e-12));
    ASSERT_DOUBLE_EQ(expec[0], a[2][0]);
    ASSERT_DOUBLE_EQ(expec[1], a[2][1]);
    ASSERT_DOUBLE_EQ(expec[2], a[2][2]);
}

class MyBFRM: public BodyFrameRotMatrix {
public:
    virtual ~MyBFRM() {};

    virtual const RotMatrix3x3& ecToEq(PrecType, TimeUnit) const {
        const static RotMatrix3x3 m;
        return m;
    }

    virtual const RotMatrix3x3& eqToEc(PrecType, TimeUnit) const {
        const static RotMatrix3x3 m;
        return m;
    }
};

TEST(PairWiseAccelerationCalculator, doJContrib) {
    Body body0;
    body0.mu = 3.8e7;
    body0.radius = 60300;
    body0.x = {0, 0, 0};
    vector<PrecType> j;
    for (auto i=0; i<12; ++i) {
        j.push_back(250* pow(i, 6));
    }
    body0.j.reset(new vector<PrecType>(j));
    body0.bfrm.reset(new MyBFRM());
    double d = 300000;
    double y = 150000;
    double z = 1000;
    double x = sqrt(d*d - y*y - z*z);
    vector<PrecType> f {
        0, 0, 3.0, 0.5, 2.5, 0.375, 0.875, 0.0625,
        0.5625, 0.0390625, 0.0859375
    };
    auto n = body0.j->size();
    body0.c.resize(n);
    for (uint i=0; i<n ; ++i) {
        body0.c[i] = f[i]*body0.j->operator[](i)*body0.mu;
    }
    Body body1;
    body1.mu = 2;
    body1.x = {x, y, z};
    vector<Body> bodies {body0, body1};
    UnthreadedDistanceCalculator udc;
    PairWiseAccelerationCalculator pwac(&udc, &bodies);
    Vector a;
    pwac.doJContrib(a, d, d*d, body1.x, body0);
    Vector expec(
        0.30655222076432748, 0.17698931821580446,
        0.21462617201824341
    );
    ASSERT_DOUBLE_EQ(expec[0], a[0]);
    ASSERT_DOUBLE_EQ(expec[1], a[1]);
    ASSERT_DOUBLE_EQ(expec[2], a[2]);
}


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


