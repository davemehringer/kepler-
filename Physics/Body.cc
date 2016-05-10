//# Body.cc
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


