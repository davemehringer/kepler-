/*
 * JPLHorizonsRetriever.h
 *
 *  Created on: Sep 2, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_JPLHORIZONSRETRIEVER_H_
#define KEPLER_JPLHORIZONSRETRIEVER_H_

#include <Math/Types.h>

#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

namespace kepler {

class Body;
class Vector;
struct Elements;

class JPLHorizonsRetriever {

public:

	enum OutputType {
		// state vectors
		VECTORS,
		// orbital elements
		ELEMENTS
	};

	enum RefPlane {
		// equatorial plane of central body
		BODY,
		ECLIPTIC,
		// indicates the plane has not been set
		NULL_PLANE
	};

	JPLHorizonsRetriever(OutputType type);

	~JPLHorizonsRetriever();

	// add information to get. Writes input file.
	void add(
		const string& body, const string& centerBody,
		PrecType time, RefPlane refPlane
	);

	vector<Body> getBodies() const;

	vector<Elements> getElements() const;

	// retrieve (if necessary) and parse the data from JPL
	// Horizons
	void retrieve();

private:

	struct BodyStruct {
		BodyStruct() : body(), centerBody(), inputFile(), jplID(-1), name(),
				outputFile(), refPlane(NULL_PLANE), time(-1) {}
		shared_ptr<Body> body;
		string centerBody;
		string inputFile;
		int jplID;
		string name;
		string outputFile;
		RefPlane refPlane;
		PrecType time;
	};

	OutputType _outputType;
	vector<BodyStruct> _bodyStruct;
	vector<Body> _bodies;
	vector<Elements> _elements;

	static const map<int, string> _refPlaneString, _outTypeString;

	void _doBodies();

	void _doElements();

	static bool _exists(const string& filename);

	static int _staticRetrieve(const string& cmd);

	static vector<string> _getLines(const string& file, int count);

};



}

#endif
