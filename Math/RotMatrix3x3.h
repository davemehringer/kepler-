/*
 * RotMatrix3x3.h
 *
 *  Created on: Sep 10, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_RotMatrix3x3_H_
#define KEPLER_RotMatrix3x3_H_

#include "Vector.h"

#include <array>
#include <memory>

using namespace std;

namespace kepler {

class RotMatrix3x3 {
public:

    // 3x3 identity matrix
    RotMatrix3x3();

    RotMatrix3x3(const array<PrecType, 9>& ary);

    ~RotMatrix3x3();

    // Matrix multiplication
    RotMatrix3x3 operator*(const RotMatrix3x3& other) const;

    bool operator==(const RotMatrix3x3& other) const;

    inline const PrecType& operator()(int row, int col) const {
        return _m[row][col];
    }

    // is this the identity matrix?
    bool isIdentity() const;

    // returns the transpose of the Matrix, since the inverse
    // and transposed Matrix are equal for rotation matrices
    const RotMatrix3x3& inverse() const;

    // transpose the matrix
    const RotMatrix3x3& transpose() const;

private:
    array<array<PrecType, 3>, 3> _m;
    mutable shared_ptr<RotMatrix3x3> _trans;

    // check determinant
    void _checkDet() const;

    // compute the inverse of this matrix, for checking value
    // is same as transpose()
    void _checkInverseAndTranspose() const;
};

Vector operator*(const Vector& v, const RotMatrix3x3& m);

ostream& operator<<(ostream& os, const RotMatrix3x3& m);

bool near(const RotMatrix3x3&u, const RotMatrix3x3& v, PrecType epsilon);

}

#endif
