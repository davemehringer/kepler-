/*
 * RotMatrix3x3.cc
 *
 *  Created on: Sep 10, 2015
 *      Author: dmehring
 */

#include <Math/RotMatrix3x3.h>

#include <Exception/KeplerException.h>
#include <Math/Math.h>

#include <iomanip>
#include <sstream>

using namespace std;

namespace kepler {

RotMatrix3x3::RotMatrix3x3() {
    _m[0][0] = 1;
    _m[0][1] = 0;
    _m[0][2] = 0;
    _m[1][0] = 0;
    _m[1][1] = 1;
    _m[1][2] = 0;
    _m[2][0] = 0;
    _m[2][1] = 0;
    _m[2][2] = 1;
}

RotMatrix3x3::RotMatrix3x3(const array<PrecType, 9>& ary)
: _m( ), _trans() {
    int i = 0, j = 0;
    for (auto a : ary) {
        _m[i][j] = a;
        ++j;
        if (j == 3) {
            j = 0;
            ++i;
        }
    }
    _checkDet();
    _checkInverseAndTranspose();
}

RotMatrix3x3::~RotMatrix3x3() {}

RotMatrix3x3 RotMatrix3x3::operator*(const RotMatrix3x3& other) const {
    RotMatrix3x3 r;
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            r._m[i][j] = _m[i][0]*other._m[0][j]
                + _m[i][1]*other._m[1][j]
                + _m[i][2]*other._m[2][j];
        }
    }
    return r;
}

bool RotMatrix3x3::operator==(const RotMatrix3x3& other) const {
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            if (_m[i][j] != other._m[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void RotMatrix3x3::_checkDet() const {
    PrecType det =  _m[0][0] * (
        _m[1][1]*_m[2][2] - _m[1][2]*_m[2][1]
    ) + _m[0][1] * (
        _m[1][2]*_m[2][0] - _m[1][0]*_m[2][2]
    ) + _m[0][2] * (
        _m[1][0]*_m[2][1] - _m[1][1]*_m[2][0]
    );
    if (! near(det, 1.0, 1e-11)) {
        ostringstream d;
        d << setprecision(20) << det;
        cerr << d.str() << endl;
        cerr << "det is " + d.str() + ", != 1, not a rotation matrix" << endl;
        throw KeplerException();
    }
}

const RotMatrix3x3& RotMatrix3x3::inverse() const {
    return transpose();
}

void RotMatrix3x3::_checkInverseAndTranspose() const {
    RotMatrix3x3 inv;
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
            inv._m[i][j];
        }
    }
    if (! near(inv, transpose(), 8e-10)) {
        cerr << setprecision(20) << inv << endl;
        cerr << setprecision(20) << transpose() << endl;
        cerr << "inverse != transpose, not a rotation matrix" << endl;
        throw KeplerException();
    }
}


bool RotMatrix3x3::isIdentity() const {
    PrecType epsilon = std::numeric_limits<PrecType>::epsilon();
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

const RotMatrix3x3& RotMatrix3x3::transpose() const {
    if (! _trans) {
        _trans.reset(new RotMatrix3x3());
        _trans->_m[0][0] = _m[0][0];
        _trans->_m[0][1] = _m[1][0];
        _trans->_m[0][2] = _m[2][0];
        _trans->_m[1][0] = _m[0][1];
        _trans->_m[1][1] = _m[1][1];
        _trans->_m[1][2] = _m[2][1];
        _trans->_m[2][0] = _m[0][2];
        _trans->_m[2][1] = _m[1][2];
        _trans->_m[2][2] = _m[2][2];
    }
    return *_trans;
}

Vector operator*(const Vector& v, const RotMatrix3x3& m) {
    return Vector(
            v[0]*m(0,0) + v[1]*m(1,0) + v[2]*m(2,0),
            v[0]*m(0,1) + v[1]*m(1,1) + v[2]*m(2,1),
            v[0]*m(0,2) + v[1]*m(1,2) + v[2]*m(2,2)
    );
}

ostream& operator<<(ostream& os, const RotMatrix3x3& m) {
    os << "/" << m(0, 0) << ", " << m(0, 1) << ", " << m(0, 2) << "\\" << endl;
    os << "|" << m(1, 0) << ", " << m(1, 1) << ", " << m(1, 2) << "|" << endl;
    os << "\\" << m(2, 0) << ", " << m(2, 1) << ", " << m(2, 2) << "/";
    return os;
}

bool near(const RotMatrix3x3&u, const RotMatrix3x3& v, PrecType epsilon) {
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
