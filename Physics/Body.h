#ifndef __KEPLER_BODY
#define __KEPLER_BODY

#include <memory>
#include <string>
#include <vector>

#include <Math/Vector.h>

using namespace std;

namespace kepler {

class BodyFrameRotMatrix;

// reference frame of x and v
	enum Frame {
		ECLIPTIC,
		ORIGIN_BDOY_EQUATORIAL
	};

	// origin of coordinate system
	enum Origin {
		ORIGIN_BODY,
		SYSTEM_BARYCENTER
	};

	struct Body {
		// acceleration vector
		Vector a = Vector();
		vector<PrecType> c = vector<PrecType>();
		// central body (normally body which is being orbited, used
		// for force calculations associated with zonal harmonics of or orbital elements
		// relative to the central body). nullptr => This body does not "orbit" another body,
		// eg in the case of the sun.
		shared_ptr<Body> centerBody = shared_ptr<Body>() ;
		shared_ptr<BodyFrameRotMatrix> bfrm = nullptr;
		Frame frame = ECLIPTIC;
		// the body id. It is important that each body in the system
		// have a unique id. And it is up to the caller to specify those
		// ids
		int id = 0;
		// zonal harmonic coefficients, if any (nullptr if none)
		shared_ptr<vector<PrecType> > j = nullptr;
		// _mu = G*mass
		PrecType mu = 0;
		// body name
		string name = "";
		Origin origin = SYSTEM_BARYCENTER;
		// effective radius, used for computation of forces associated
		// with harmonic coefficients
		PrecType radius = 0;
		// velocity vector
		Vector v = Vector();
		// position vector
		Vector x = Vector();
	};

ostream& operator<<(ostream& os, const Body& b);

}
#endif

