/*
 * tJPLHorizonsRetriever.cc
 *
 *      Author: dmehring
 */

#include <gtest/gtest.h>
#include <SolarSystem/JPLHorizonsRetriever.h>

#include <Physics/Body.h>
#include <BodyFrameRotMatrix/BodyFrameRotMatrix.h>
#include <Math/Constants.h>
#include <Math/Math.h>
#include <Math/RotMatrix3x3.h>
#include <Math/Vector.h>
#include <Physics/StateVectorsOrbitalElements.h>

using namespace kepler;

TEST(JPLHorizonsRetrieverTest, retrieve) {
	{
		JPLHorizonsRetriever jhr(JPLHorizonsRetriever::VECTORS);
		jhr.add("jupiter", "sun", 2456733, JPLHorizonsRetriever::ECLIPTIC);
		jhr.add("jupiter", "sun", 2456733, JPLHorizonsRetriever::BODY);
		jhr.add("saturn", "sun", 2456733, JPLHorizonsRetriever::ECLIPTIC);
		jhr.retrieve();
		auto bodies = jhr.getBodies();
		int i = 0;
		Vector expr, expv;
		for (const auto& b : bodies) {
			switch (i) {
			case 0:
				expr = Vector(-1.872657076351025, 4.872082061238438,  2.166947626111965E-02);
				expv = Vector(-7.141646521668177E-03, -2.352165608261079E-03,  1.695742510177329E-04);
				break;
			case 1:
				expr = Vector(-5.594740786524897E-01,  5.177133746040533, -3.588490409275931E-01);
				expv = Vector( -7.478796683089576E-03, -4.816581137423233E-04, -6.325813388206591E-04);
				break;
			case 2:
				expr = Vector(-6.604556228059373, -7.360437437593992,  3.909099648465508E-01);
				expv = Vector(3.842693382139356E-03, -3.742478541994696E-03, -8.798694297395055E-05);
				break;
			default:
				break;
			}
			// ASSERT_DOUBLE_EQ(expr, b.x/KM_PER_AU);
            ASSERT_DOUBLE_EQ(expr[0], b.x[0]/KM_PER_AU);
            ASSERT_DOUBLE_EQ(expr[1], b.x[1]/KM_PER_AU);
            ASSERT_DOUBLE_EQ(expr[2], b.x[2]/KM_PER_AU);
			//ASSERT_EQ(expv, b.v/KMPERSEC_PER_AUPERDAY);
            ASSERT_DOUBLE_EQ(expv[0], b.v[0]/KMPERSEC_PER_AUPERDAY);
            ASSERT_DOUBLE_EQ(expv[1], b.v[1]/KMPERSEC_PER_AUPERDAY);
            ASSERT_DOUBLE_EQ(expv[2], b.v[2]/KMPERSEC_PER_AUPERDAY);
			++i;
		}
	}
	{
	    PrecType t = 2457733;
	    JPLHorizonsRetriever jhr(JPLHorizonsRetriever::VECTORS);
        jhr.add("megaclite", "jupiter", t, JPLHorizonsRetriever::ECLIPTIC);
        jhr.add("megaclite", "jupiter", t, JPLHorizonsRetriever::BODY);
        jhr.add("adrastea", "jupiter", t, JPLHorizonsRetriever::ECLIPTIC);
        jhr.add("adrastea", "jupiter", t, JPLHorizonsRetriever::BODY);
        jhr.add("metis", "jupiter", t, JPLHorizonsRetriever::ECLIPTIC);
        jhr.add("metis", "jupiter", t, JPLHorizonsRetriever::BODY);
        jhr.add("himalia", "jupiter", t, JPLHorizonsRetriever::ECLIPTIC);
        jhr.add("himalia", "jupiter", t, JPLHorizonsRetriever::BODY);
        jhr.add("amalthea", "jupiter", t, JPLHorizonsRetriever::ECLIPTIC);
        jhr.add("amalthea", "jupiter", t, JPLHorizonsRetriever::BODY);
        jhr.add("phoebe", "saturn", t, JPLHorizonsRetriever::ECLIPTIC);
        jhr.add("phoebe", "saturn", t, JPLHorizonsRetriever::BODY);
        jhr.add("titan", "saturn", t, JPLHorizonsRetriever::ECLIPTIC);
        jhr.add("titan", "saturn", t, JPLHorizonsRetriever::BODY);
        jhr.add("janus", "saturn", t, JPLHorizonsRetriever::ECLIPTIC);
        jhr.add("janus", "saturn", t, JPLHorizonsRetriever::BODY);

        jhr.retrieve();
        auto bodies = jhr.getBodies();
        int n = bodies.size()/2;
        for (auto i=0; i<n; ++i) {
            auto j = 2*i;
            auto xec = bodies[j].x;
            auto ectoeq = bodies[j].centerBody->bfrm->ecToEq(t, DAY);
            auto xeq = xec * ectoeq;
            cout << setprecision(20) << xeq << endl;
            cout << bodies[j+1].x << endl;
            ASSERT_TRUE(near(bodies[j+1].x, xeq, 1e-6, 1e-6));
            auto eqtoec = bodies[j].centerBody->bfrm->eqToEc(t, DAY);
            auto yeq = bodies[j+1].x;
            auto yec = yeq*eqtoec;
            ASSERT_TRUE(near(bodies[j].x, yec, 1e-6, 1e-6));

        }

	}
	{
	    JPLHorizonsRetriever jhr1(JPLHorizonsRetriever::ELEMENTS);
	    jhr1.add("jupiter", "sun", 2456733, JPLHorizonsRetriever::ECLIPTIC);
	    jhr1.add("jupiter", "sun", 2456733, JPLHorizonsRetriever::BODY);
	    jhr1.add("saturn", "sun", 2456733, JPLHorizonsRetriever::ECLIPTIC);
	    jhr1.retrieve();
	    auto elements = jhr1.getElements();
	    for (const auto e: elements) {
	        cout << e << endl;
	    }
	    PrecType epsilon = 1e-15;
	    ASSERT_TRUE(nearAbs(elements[0].e, 4.895227635734191E-02, epsilon));
	    ASSERT_TRUE(nearAbs(elements[0].i, 1.303752776886219, epsilon));
	    ASSERT_TRUE(nearAbs(elements[0].node,1.005144333645579E+02 , epsilon));
	    ASSERT_TRUE(nearAbs(elements[0].peri, 2.739656560583393E+02, epsilon));
	    ASSERT_TRUE(nearAbs(elements[0].mAnomaly,9.095344042061248E+01 , epsilon));
	    cout << "*** a " << elements[0].a << endl;
	    ASSERT_TRUE(near(elements[0].a,5.202958543306223, epsilon));

	    ASSERT_TRUE(nearAbs(elements[1].e, 4.895227635734195E-02, epsilon));
        ASSERT_TRUE(nearAbs(elements[1].i, 6.092087112825052, epsilon));
        ASSERT_TRUE(nearAbs(elements[1].node, 2.359511983801610E+02, epsilon));
        ASSERT_TRUE(nearAbs(elements[1].peri, 1.238290990149945E+02, epsilon));
        ASSERT_TRUE(nearAbs(elements[1].mAnomaly, 9.095344042061222E+01, epsilon));
        ASSERT_TRUE(near(elements[1].a, 5.202958543306223, epsilon));

        ASSERT_TRUE(nearAbs(elements[2].e, 0.055072607934018649734 , epsilon));
        ASSERT_TRUE(nearAbs(elements[2].i,2.487826389399676 , epsilon));
        ASSERT_TRUE(nearAbs(elements[2].node,1.135757486420357E+02 , epsilon));
        ASSERT_TRUE(nearAbs(elements[2].peri, 339.03074440815072421 , epsilon));
        ASSERT_TRUE(nearAbs(elements[2].mAnomaly, 130.91304312584969693, epsilon));
        ASSERT_TRUE(near(elements[2].a, 9.5372805208152726664, epsilon));
	}
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


