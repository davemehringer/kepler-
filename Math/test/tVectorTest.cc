/*
 * tVector.cc
 *
 *  Created on: Aug 29, 2015
 *      Author: dmehring
 */

#include <gtest/gtest.h>
#include <Math/VectorTester.h>

#include <OS/Timer.h>

#include <chrono>
#include <thread>

using namespace kepler;

TEST(VectorTest, populate) {
	VectorTester vt(100000000);
	Timer t;
	t.start();
	//vt.populate(0, 100000000);

	vector<unique_ptr<thread>> threads;
	for (int i=0; i<8; ++i) {
	    int start =12500000*i;
	    int end =   12500000 + start;
	    threads.push_back(unique_ptr<thread>(new thread {&VectorTester::populate, &vt, start, end }));
	}
	for (int i=0; i<8; ++i) {
	    threads[i]->join();
	}

	t.stop();
	cout << "time " << t.totalDuration() << endl;
	const auto& v = vt.getVector();
	int n = v.size();
	cout << "v size " << n << endl;
	int i = 0;
	for (const auto& z: v) {
	    ASSERT_EQ(i, z);
	    ++i;
	}

}

int main(int argc, char **argv) {
    Timer t;
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


