//# tVector.cc
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
#include <Math/Vector.h>

#include <chrono>

using namespace kepler;

TEST(VectorTest, Constructors) {
	Vector v;
	ASSERT_EQ(0, v[0]);
	ASSERT_EQ(0, v[1]);
	ASSERT_EQ(0, v[2]);
	Vector w(4, 5, 6);
	ASSERT_EQ(4, w[0]);
	ASSERT_EQ(5, w[1]);
	ASSERT_EQ(6, w[2]);
	std::array<PrecType, 3> ary {7, 8, 9};
	Vector x(ary);
	ASSERT_EQ(7, x[0]);
	ASSERT_EQ(8, x[1]);
	ASSERT_EQ(9, x[2]);
}

TEST(VectorTest, SimpleMath) {
	Vector v(4, 5, 6);
	auto w = 4*v;
	ASSERT_EQ(16, w[0]);
	ASSERT_EQ(20, w[1]);
	ASSERT_EQ(24, w[2]);
	w = v/2;
	ASSERT_EQ(2, w[0]);
	ASSERT_EQ(2.5, w[1]);
	ASSERT_EQ(3, w[2]);

	auto k = v + w;
	ASSERT_EQ(6, k[0]);
	ASSERT_EQ(7.5, k[1]);
	ASSERT_EQ(9, k[2]);

	k = v - w;
	ASSERT_EQ(Vector(2, 2.5, 3), k);

	k = Vector();
	v.subtract(k, w);
    ASSERT_EQ(Vector(2, 2.5, 3), k);

	Vector m(6, 7, 8);
	m += v;
	ASSERT_EQ(10, m[0]);
	ASSERT_EQ(12, m[1]);
	ASSERT_EQ(14, m[2]);

	m = Vector(6, 7, 8);
	m -= v;
	ASSERT_EQ(2, m[0]);
	ASSERT_EQ(2, m[1]);
	ASSERT_EQ(2, m[2]);

	m = Vector(6, 8, 10);
	m *= 2;
	ASSERT_EQ(12, m[0]);
    ASSERT_EQ(16, m[1]);
    ASSERT_EQ(20, m[2]);

    m = Vector(6, 8, 10);
    m /= 2;
    ASSERT_EQ(3, m[0]);
    ASSERT_EQ(4, m[1]);
    ASSERT_EQ(5, m[2]);
}

TEST(VectorTest, dot) {
	Vector v(4, 5, 6);
	Vector w(3, 2, 1);
	auto x = v.dot(w);
	ASSERT_EQ(28, x);
	x = w.dot(v);
	ASSERT_EQ(28, x);
}

TEST(VectorTest, mag) {
	Vector v(4, 5, 6);
	auto x = v.mag();
	ASSERT_DOUBLE_EQ(sqrt(77), x);
}

TEST(VectorTest, mag2) {
	Vector v(4, 5, 6);
	auto x = v.mag2();
	ASSERT_EQ(77, x);
}

TEST(VectorTest, negate) {
	Vector v(4, 5, 6);
	auto x = v.negate();
	ASSERT_EQ(-4, x[0]);
	ASSERT_EQ(-5, x[1]);
	ASSERT_EQ(-6, x[2]);
}

TEST(VectorTest, unit) {
	Vector v(4, 5, 6);
	auto x = v.unit();
	ASSERT_DOUBLE_EQ(4/sqrt(77), x[0]);
	ASSERT_DOUBLE_EQ(5/sqrt(77), x[1]);
	ASSERT_DOUBLE_EQ(6/sqrt(77), x[2]);
	ASSERT_DOUBLE_EQ(1, x.mag());
}

TEST(VectorTest, near) {
	Vector u(4, 5, 6);
	Vector v(4, 5, 6);
	//auto x = acos(u.dot(v)/u.mag()/v.mag());
	ASSERT_TRUE(near(u, v, 1e-8, 1e-7));
	u[0] = 20;
	ASSERT_FALSE(near(u, v, 1e-8, 1e-7));
	u[1] = 21;
	u[2] = 22;
	v[0] = 21;
	v[1] = 22;
	v[2] = 23;
	ASSERT_TRUE(near(u, v, 1e-1, 1e-1));
	ASSERT_FALSE(near(u, v, 1e-2, 1e-2));
}

TEST(VectorTest, sabs) {
    Vector u(4, 5, 6);
    u.sabs();
    ASSERT_EQ(Vector(4, 5, 6), u);
    u = Vector(-4, -5, -6);
    u.sabs();
    ASSERT_EQ(Vector(4, 5, 6), u);
}


TEST(VectorTest, max) {
    Vector u(4, 5, 6);
    ASSERT_EQ(6, u.max());
    u = Vector(-4, -5, -6);
    ASSERT_EQ(-4, u.max());
}


TEST(VectorTest, VvectorAdd) {
    Vector u(1, 2, 3);
    Vector v (4, 5, 6);
    Vector w(7, 8, 9);
    Vector x(10, 11, 12);
    Vvector a {u, v};
    Vvector b {w, x};
    Vvector c(a.size());
    add(c, a, b);
    ASSERT_EQ(Vector(8, 10, 12), c[0]);
    ASSERT_EQ(Vector(14, 16, 18), c[1]);
}

TEST(VectorTest, VvectorTimes) {
    Vector u(1, 2, 3);
    Vector v (4, 5, 6);
    Vvector a {u, v};
    //Vvector b {w, x};
    Vvector c(a.size());
    times(c, a, 3);
    ASSERT_EQ(Vector(3, 6, 9), c[0]);
    ASSERT_EQ(Vector(12, 15, 18), c[1]);
}

TEST(VectorTest, timings) {
    /*
    {
        chrono::steady_clock::time_point start, stop;
        Vector x(2, 3, 4);
        PrecType d = 6;
        auto nLoops = 1000000;
        start = chrono::steady_clock::now();
        for (auto i=0; i<nLoops; ++i) {
            auto z = d*x;
        }
        stop = chrono::steady_clock::now();
        cout << "Duration "  << chrono::duration_cast<chrono::milliseconds>(stop - start).count()
            << " milliseconds." << endl;
    }

    {
        chrono::steady_clock::time_point start, stop;
        Vector x(2, 3, 4);
        PrecType d = 6;
        auto nLoops = 1000000;
        Vector y;
        start = chrono::steady_clock::now();
        for (auto i=0; i<nLoops; ++i) {
            y = d*x;
        }
        stop = chrono::steady_clock::now();
        cout << "Duration "  << chrono::duration_cast<chrono::milliseconds>(stop - start).count()
            << " milliseconds." << endl;
    }
    */
    {
        chrono::steady_clock::time_point start, stop;
        Vector x(2, 3, 4);
        PrecType d = 6;
        auto nLoops = 1000000;
        start = chrono::steady_clock::now();
        Vector b;
        for (auto i=0; i<nLoops; ++i) {
            x.times(b, d);
        }
        stop = chrono::steady_clock::now();
        cout << "Duration "  << chrono::duration_cast<chrono::milliseconds>(stop - start).count()
            << " milliseconds." << endl;
    }
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


