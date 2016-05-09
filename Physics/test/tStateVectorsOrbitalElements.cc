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


