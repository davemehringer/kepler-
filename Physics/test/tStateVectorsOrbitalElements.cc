//# tStateVectorsOrbitalElements.cc
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
 * tJPLHorizonsRetriever.cc
 *
 *      Author: dmehring
 */

#include <gtest/gtest.h>

#include <Physics/StateVectorsOrbitalElements.h>

#include <Physics/Body.h>
#include <SolarSystem/JPLHorizonsRetriever.h>
#include <SolarSystem/SSObjects.h>

using namespace kepler;

TEST(StateVectorsOrbitalElementsTest, test) {
    {
		JPLHorizonsRetriever jhr(JPLHorizonsRetriever::VECTORS);
		jhr.add("jupiter", "sun", 2456733, JPLHorizonsRetriever::ECLIPTIC);
		jhr.retrieve();
        JPLHorizonsRetriever jhr1(JPLHorizonsRetriever::ELEMENTS);
        jhr1.add("jupiter", "sun", 2456733, JPLHorizonsRetriever::ECLIPTIC);
        jhr1.retrieve();

		auto bodies = jhr.getBodies();
		auto jupiter = bodies[0];
		auto sun = SSObjects::createBody("sun");

		auto els = vectorsToElements(sun.mu, jupiter.mu, jupiter.x, jupiter.v);
		cout << setprecision(20) << els << endl;
		cout << setprecision(20) << jhr1.getElements()[0] << endl;
		ASSERT_TRUE(near(jhr1.getElements()[0], els, 5e-7));

		auto vecs = kepler::elementsToVectors(sun.mu, jupiter.mu, els);
		cout << setprecision(20) << vecs.first << endl;
		cout << setprecision(20) << jupiter.x << endl;
		cout << setprecision(20) << vecs.second << endl;
		cout << setprecision(20) << jupiter.v << endl;
    }
    {
        JPLHorizonsRetriever jhr(JPLHorizonsRetriever::VECTORS);
        jhr.add("moon", "earth", 2456733, JPLHorizonsRetriever::ECLIPTIC);
        jhr.retrieve();
        JPLHorizonsRetriever jhr1(JPLHorizonsRetriever::ELEMENTS);
        jhr1.add("moon", "earth", 2456733, JPLHorizonsRetriever::ECLIPTIC);
        jhr1.retrieve();

        auto bodies = jhr.getBodies();
        auto moon = bodies[0];
        auto earth = SSObjects::createBody("earth");

        auto els = vectorsToElements(earth.mu, moon.mu, moon.x, moon.v);
        cout << setprecision(20) << els << endl;
        cout << setprecision(20) << jhr1.getElements()[0] << endl;
        ASSERT_TRUE(near(jhr1.getElements()[0], els, 7e-6));

        auto vecs = kepler::elementsToVectors(earth.mu, moon.mu, els);
        cout << setprecision(20) << vecs.first << endl;
        cout << setprecision(20) << moon.x << endl;
        cout << setprecision(20) << vecs.second << endl;
        cout << setprecision(20) << moon.v << endl;
    }

}


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


