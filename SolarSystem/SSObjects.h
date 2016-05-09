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
