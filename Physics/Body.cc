/*
 * Body.cc
 *
 *  Created on: Sep 26, 2015
 *      Author: dmehring
 */

#include <Physics/Body.h>

namespace kepler {

ostream& operator<<(ostream& os, const Body& b) {
    os << "id " << b.id << endl;
    os << "name: " << b.name << endl;
    os << "mu " << b.mu << endl;
    os << "radius " << b.radius << endl;
    if (b.centerBody) {
        os << "center body: " << b.centerBody->name << endl;
    }
    os << "origin: " << b.origin << endl;
    os << "frame: " << b.frame << endl;
    if (b.j) {
        os << "j values ptr " << b.j << endl;
    }
    if (b.bfrm) {
        os << "rotation matrix ptr: " << b.bfrm << endl;
    }
    os << "x: " << b.x << endl;
    os << "v: " << b.v << endl;
    os << "a: " << b.a << endl;
    return os;
}

}


