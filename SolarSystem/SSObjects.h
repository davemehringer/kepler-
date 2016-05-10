//# SSObjects.h
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
 * SSObjects.h
 *
 *  Created on: Aug 30, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_SSOBJECTS_H_
#define KEPLER_SSOBJECTS_H_

#include <map>
#include <string>
#include <vector>

using namespace std;

namespace kepler {

class Body;

class SSObjects {
	// static interface to retrieve data on solar system objects. Data
	// is from JPL Horizons.

public:

	// Factory method to create a Body object using data of the
	// specified solar system object. Position and velocity vectors
	// will be uninitialized, so must be set by the caller after this
	// method call.
	static Body createBody(const string& name);

	// get the JPL ID of the object of given name.
	static int jplID(const string& name);

private:
	// jplID -> Body map
	static map<int, Body> _body;

	static map<string, int> _nameToID;

	static void _initBodies();
};

}

#endif
