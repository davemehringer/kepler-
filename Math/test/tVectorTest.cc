//# tVectorTest.cc
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


