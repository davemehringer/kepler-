/*
 * tVector.cc
 *
 *  Created on: Aug 29, 2015
 *      Author: dmehring
 */

#include <gtest/gtest.h>

#include <TimeStepManager/AccelerationTimeStepManager.h>

using namespace kepler;

TEST(AccelerationTimeStepManager, modify) {
	Vector b0(5, 5, 5);
	Vector b1(5, 5, 5);
	Vector b2(5,5,5);
	Vvector initial {b0, b1, b2};

    Vector c0(6, 6, 6);
    Vector c1(6, 6, 6);
    Vector c2(6,6,6);
    Vvector final {c0, c1, c2};

    TimeStepManager::TimeStepManagerData tsmd;
    tsmd.aInitial = &initial;
    tsmd.aFinal = &final;
    AccelerationTimeStepManager atsm(1.0, 2.0);
    ASSERT_EQ(TimeStepManager::INCREASE, atsm.modify(tsmd));

    c2[2] = -5;
    final[2] = c2;
    tsmd.aFinal = &final;
    ASSERT_EQ(TimeStepManager::NO_CHANGE, atsm.modify(tsmd));

    c2[2] = 25;
    final[2] = c2;
    tsmd.aFinal = &final;
    ASSERT_EQ(TimeStepManager::DECREASE, atsm.modify(tsmd));


}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


