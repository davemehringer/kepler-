//# tPositionPublisher.cc
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
 * tMath.cc
 *
 *      Author: dmehring
 */

#include <gtest/gtest.h>
#include <Integrator/PositionPublisher.h>

#include <Math/Vector.h>
#include <fcntl.h>

using namespace std;

using namespace kepler;

TEST(PositionPublisherTest, setX) {
    const auto fifo = "/tmp/fifo_kepler";
    if (access( fifo, F_OK ) != -1) {
        auto res = unlink(fifo);
        if (res < 0) {
            cerr << "Cannot unlink " << fifo << ": " << strerror(errno) << endl;
            ASSERT_TRUE(res >= 0);
        }
    }
	PositionPublisher pp;
	ASSERT_FALSE(pp.canWrite());
	cout << "mkfifo" << endl;
	auto res = mkfifo(fifo, S_IRUSR | S_IWUSR);
	if (res < 0) {
	    cerr << "Cannot mkfifo " << fifo << " : " << strerror(errno) << endl;
	}
	ASSERT_TRUE(res >= 0);

	cout << "open for reading" << endl;
	res = open(fifo, O_RDONLY | O_NONBLOCK);
	if (res < 0) {
	    cerr << "Cannot open " << fifo << " for reading: " << strerror(errno) << endl;
	}
	cout << "create pp1" << endl;
	PositionPublisher pp1;
	cout << "can write" << endl;
	ASSERT_TRUE(pp1.canWrite());
	Vvector v(1);
	v.push_back(Vector(1,2,3));
	cout << "setX" << endl;
	pp1.setX(v);
	char buffer[90];
	read(  res,    buffer,  90 );
	cout << "buffer " << buffer << endl;
	pp1.end();
	close(res);
	if (unlink(fifo) < 0) {
	    cerr << "Cannot unlink " << fifo << ": " << strerror(errno) << endl;
	}
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


