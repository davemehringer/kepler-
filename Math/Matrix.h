/*
 * Matrix3x3.h
 *
 *  Created on: Aug 30, 2015
 *      Author: dmehring
 */

#include <array>

#ifndef KEPLER_MATRIX_H_
#define KEPLER_MATRIX_H_

#include "Vector.h"

using namespace std;

namespace kepler {


// represents a  matrix

template <class T> class Matrix

public:

	Matrix3x3() = delete;

	Matrix3x3(const array<array<double, 3>, 3>& matrix);

	// first three => first row, second three => second row
	// third three => third row. For easy initialization
	Matrix3x3(const array<double, 9>& ary);

	virtual ~Matrix3x3();

	// Matrix multiplication
	Matrix3x3 operator*(const Matrix3x3& other) const;

	//Vector operator*(const Vector& v) const;

	bool operator==(const Matrix3x3& other) const;

	inline const double& operator()(int row, int col) const {
		return _m[row][col];
	}

	virtual inline double& operator()(int row, int col) {
		return _m[row][col];
	}

	// determinant
	double det() const;

	// compute the inverse of this matrix
	Matrix3x3 inverse() const;

	// is this the identity matrix?
	bool isIdentity() const;

	// transpose the matrix
	Matrix3x3 transpose() const;

private:
	array<array<double, 3>, 3> _m;
	mutable Vector _s;
};

Vector operator*(const Vector& v, const Matrix3x3& m);

ostream& operator<<(ostream& os, const Matrix3x3& m);

bool near(const Matrix3x3&u, const Matrix3x3& v, double epsilon);

}

#endif
