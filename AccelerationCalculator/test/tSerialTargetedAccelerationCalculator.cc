/*
 * tVector.cc
 *
 *  Created on: Aug 29, 2015
 *      Author: dmehring
 */

#include <gtest/gtest.h>
#include <AccelerationCalculator/SerialTargetedAccelerationCalculator.h>

#include <Physics/Body.h>
#include <DistanceCalculator/SerialTargetedDistanceCalculator.h>

using namespace kepler;

TEST(SerialTargetedAccelerationCalculator, compute) {
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
	SerialTargetedDistanceCalculator stdc;
	SerialTargetedAccelerationCalculator stac(&stdc, &bodies);
	set<int> targets;
	targets.insert(1);
	stac.setTargetBodies(&targets);
	stac.compute(a, &x);
	/*
	Vector expec {
	    0.0081027416951023491,
	    -0.0028128978401775126,
	    -0.0024769460147473221
	};
	*/
	Vector expec;
	cout << expec << endl;
	    cout << a[0] << endl;
	    ASSERT_EQ(expec, a[0]);

	expec = Vector(
	    0.007745739105366526,
	    -0.011230295747089588,
	    0.014714852388812649
	);
	cout << expec << endl;
	cout << a[1] << endl;
    ASSERT_TRUE(near(expec, a[1], 1e-12, 1e-12));

    expec = Vector();
    ASSERT_EQ(expec, a[2]);

}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


