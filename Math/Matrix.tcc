/*
 * Matrix3x3.cc
 *
 *  Created on: Aug 30, 2015
 *      Author: dmehring
 */

#include "Math.h"

#include <cmath>
#include <iostream>
#include <limits>
#include "Matrix.h"

namespace kepler {

Matrix3x3::Matrix3x3() : _m() {
	for (auto row : _m) {
		for (auto el : row) {
			el = 0;
		}
	}
}

Matrix3x3::Matrix3x3(
	const array<array<double, 3>, 3>& matrix
) : _m(matrix) {}

Matrix3x3::Matrix3x3(const array<double, 9>& ary) : _m() {
	int i = 0, j = 0;
	for (auto a : ary) {
		_m[i][j] = a;
		++j;
		if (j == 3) {
			j = 0;
			++i;
		}
	}
}

Matrix3x3::~Matrix3x3() {}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& other) const {
	Matrix3x3 r;
	for (int i=0; i<3; ++i) {
		for (int j=0; j<3; ++j) {
			r._m[i][j] = _m[i][0]*other._m[0][j]
				+ _m[i][1]*other._m[1][j]
				+ _m[i][2]*other._m[2][j];
		}
	}
	return r;
}
/*
Vector Matrix3x3::operator*(const Vector& v) const {
	for (int i=0; i<3; ++i) {
		_s[i] = _m[i][0]*v[0] + _m[i][1]*v[1] + _m[i][2]*v[2];
	}
	return _s;
}
*/

bool Matrix3x3::operator==(const Matrix3x3& other) const {
	for (int i=0; i<3; ++i) {
		for (int j=0; j<3; ++j) {
			if (_m[i][j] != other._m[i][j]) {
				return false;
			}
		}
	}
	return true;
}

double Matrix3x3::det() const {
    return _m[0][0] * (
		_m[1][1]*_m[2][2] - _m[1][2]*_m[2][1]
	) + _m[0][1] * (
		_m[1][2]*_m[2][0] - _m[1][0]*_m[2][2]
	) + _m[0][2] * (
		_m[1][0]*_m[2][1] - _m[1][1]*_m[2][0]
	);
}


Matrix3x3 Matrix3x3::inverse() const {
	auto d = det();
	if (d == 0) {
		throw "Matrix has no inverse, determinant is 0";
	}
	Matrix3x3 inv;
	inv._m[0][0] = _m[1][1]*_m[2][2] - _m[1][2]*_m[2][1];
	inv._m[0][1] = _m[0][2]*_m[2][1] - _m[0][1]*_m[2][2];
	inv._m[0][2] = _m[0][1]*_m[1][2] - _m[0][2]*_m[1][1];
	inv._m[1][0] = _m[1][2]*_m[2][0] - _m[1][0]*_m[2][2];
	inv._m[1][1] = _m[0][0]*_m[2][2] - _m[0][2]*_m[2][0];
	inv._m[1][2] = _m[0][2]*_m[1][0] - _m[0][0]*_m[1][2];
	inv._m[2][0] = _m[1][0]*_m[2][1] - _m[1][1]*_m[2][0];
	inv._m[2][1] = _m[0][1]*_m[2][0] - _m[0][0]*_m[2][1];
	inv._m[2][2] = _m[0][0]*_m[1][1] - _m[0][1]*_m[1][0];
	for (int i=0; i<3; ++i) {
		for (int j=0; j<3; ++j) {
			inv._m[i][j] /= d;
		}
	}
	return inv;
}

bool Matrix3x3::isIdentity() const {
	double epsilon = std::numeric_limits<double>::epsilon();
	for (int i=0; i<3; ++i) {
		for (int j=0; j<3; ++j) {
			if (i == j) {
				if (abs(_m[i][j] - 1) > epsilon) {
					return false;
				}
			}
			else {
				if (abs(_m[i][j]) > epsilon) {
					return false;
				}
			}
		}
	}
	return true;
}

Matrix3x3 Matrix3x3::transpose() const {
    return Matrix3x3(
            array<double, 9> {
                _m[0][0], _m[1][0], _m[2][0],
                _m[0][1], _m[1][1], _m[2][1],
                _m[0][2], _m[1][2], _m[2][2]
            }
    );
}

Vector operator*(const Vector& v, const Matrix3x3& m) {
    return Vector(
            v[0]*m(0,0) + v[1]*m(1,0) + v[2]*m(2,0),
            v[0]*m(0,1) + v[1]*m(1,1) + v[2]*m(2,1),
            v[0]*m(0,2) + v[1]*m(1,2) + v[2]*m(2,2)
    );
}

ostream& operator<<(ostream& os, const Matrix3x3& m) {
	os << "/" << m(0, 0) << ", " << m(0, 1) << ", " << m(0, 2) << "\\" << endl;
	os << "|" << m(1, 0) << ", " << m(1, 1) << ", " << m(1, 2) << "|" << endl;
	os << "\\" << m(2, 0) << ", " << m(2, 1) << ", " << m(2, 2) << "/";
	return os;
}

bool near(const Matrix3x3&u, const Matrix3x3& v, double epsilon) {
    for (auto i=0; i<3; ++i) {
        for (auto j=0; j<3; ++j) {
            if (! near(u(i,j), v(i,j), epsilon)) {
                return false;
            }
        }
    }
    return true;
}

}
