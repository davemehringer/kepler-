//# SSObjects.cc
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
 * SSObjects.cc
 *
 *  Created on: Aug 30, 2015
 *      Author: dmehring
 */

#include <SolarSystem/SSObjects.h>


#include <BodyFrameRotMatrix/EarthFrameRotMatrix.h>
#include <BodyFrameRotMatrix/JupiterFrameRotMatrix.h>
#include <BodyFrameRotMatrix/MarsFrameRotMatrix.h>
#include <BodyFrameRotMatrix/NeptuneFrameRotMatrix.h>
#include <BodyFrameRotMatrix/SaturnFrameRotMatrix.h>
#include <BodyFrameRotMatrix/UranusFrameRotMatrix.h>
#include <Exception/KeplerException.h>
#include <Math/Constants.h>
#include <Physics/Body.h>

namespace kepler {

map<int, Body> SSObjects::_body;
map<string, int> SSObjects::_nameToID;

Body SSObjects::createBody(const string& name) {
	return _body.find(jplID(name))->second;
}

int SSObjects::jplID(const string& name) {
	if (_nameToID.empty()) {
		_initBodies();
	}
	auto it = _nameToID.find(name);
	if (it == _nameToID.end()) {
		throw KeplerException(
			"Unknown solar system object: " + name
		);
	}
	return it->second;
}

void  SSObjects::_initBodies() {
	// in cases where JPL has a mu value, that is used.
	// Otherwise, the mass of the object times G is used
	Body b;

	shared_ptr<Body> sun(new Body());

	sun->mu = 1.3271244004193938e11;
	sun->name = "sun";
	sun->id = 10;
	_body[sun->id] = *sun;

	b = Body();
	b.centerBody = sun;
	b.mu = 22032.09;  // +/- 0.91
	b.name = "mercury";
	_body[199] = b;

	b = Body();
	b.centerBody = sun;
	b.mu = 324858.63;  // +/- 0.04
	b.name = "venus";
	_body[299] = b;

	shared_ptr<Body> earth(new Body);
	earth->bfrm.reset(new EarthFrameRotMatrix());
	earth->centerBody = sun;
	earth->j.reset(new vector<PrecType> {
	        0, 0, 1.0826254e-3, -2.532e-6, -1.619898e-6
	    });
	earth->mu = 398600.440;
	earth->name = "earth";
	// equatorial radius
	earth->radius = 6378.1;
	earth->id = 399;
	_body[earth->id] = *earth;

	b = Body();
	b.centerBody = earth;
	b.mu = 4902.798; // +/- 0.005
	b.name = "moon";
	_body[301] = b;

	shared_ptr<Body> mars(new Body());
	mars->bfrm.reset(new MarsFrameRotMatrix());
	mars->centerBody = sun;
	// from JPL email
    /*
    b.j.reset(new vector<PrecType> {
        0, 0, 1.9566086e-3, 0, -1.538684158e-5, 0,
        -4.855911997e-6, 0,  -5.956559226e-7
    });
    */
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/mar097.txt
    mars->j.reset(new vector<PrecType> {
        0, 0, 1.956608644161255E-03, 3.147495502044837E-05,
        -1.538684158075500E-05, 5.726838132552375E-06,
        -4.855911997138415E-06, -4.104248699958376E-06,
        -5.956559226330613E-07, 1.253363975517135E-06,
        -3.330918862126270E-06
    });
//	b.mu = 42828.3;   // +/- 0.1
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/mar097.txt
    mars->mu = 4.282837362069909E+04;
	mars->name = "mars";
	// mean volumetric radius from NASA
	//b.radius = 3389.5;
	// equatorial radius
    // b.radius = 3396.2;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/mar097.txt
    mars->radius = 3396;
    mars->id = 499;
	_body[mars->id] = *mars;

	b = Body();
	b.centerBody = mars;
	// b.mu = G * 1.08e16;   // +/- 0.1
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/mar097.txt
	b.mu = 7.087546066894452E-04;
	b.name = "phobos";
	_body[401] = b;

	b = Body();
	b.centerBody = mars;
	// b.mu = G * 1.80e15;   // +/- 0.15
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/mar097.txt
	b.mu = 9.615569648120313E-05;
	b.name = "deimos";
	_body[402] = b;

	shared_ptr<Body> jupiter(new Body());
	jupiter->centerBody = sun;
    jupiter->bfrm.reset(new JupiterFrameRotMatrix());
	// JPL
	// b.j.reset(new vector<PrecType> {0, 0, 1.4695625e-2, 0, -5.913139e-4, 0, 2.077511e-5});
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/jup329.txt
    jupiter->j.reset(new vector<PrecType> {
        0, 0, 1.469642900697847E-02,
        -6.436411055625769E-07, -5.871402915754995E-04,
        0, 3.425025517100406E-05
    });
	// b.mu = 126686511;     // +/- 100
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/jup329.txt
    jupiter->mu = 1.266865350495155E+08;
	jupiter->name = "jupiter";
	//Jupiter no longer from Cooper et al. 2006, Icarus, 181, 223
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/jup329.txt
	jupiter->radius = 71492;
	jupiter->id = 599;
	_body[jupiter->id] = *jupiter;

	b = Body();
	b.centerBody = jupiter;
	// b.mu = G * 893.3e20;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/jup329.txt
	b.mu = 5.959925801233187E+03;
	b.name = "io";
	_body[501] = b;

	b = Body();
	b.centerBody = jupiter;
	//b.mu = G * 479.7e20;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/jup329.txt
	b.mu = 3.202741544742959E+03;
	b.name = "europa";
	_body[502] = b;

	b = Body();
	b.centerBody = jupiter;
	// b.mu = G * 1482e20;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/jup329.txt
	b.mu = 9.887824826911430E+03;
	b.name = "ganymede";
	_body[503] = b;

	b = Body();
	b.centerBody = jupiter;
	//b.mu = G * 1076e20;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/jup329.txt
	b.mu = 7.179296583471260E+03;
	b.name = "callisto";
	_body[504] = b;

	b = Body();
	b.centerBody = jupiter;
	b.name = "amalthea";
	//b.mu = G * 2.06816243767413e18; // http://solarsystem.nasa.gov/planets/profile.cfm?Object=Amalthea
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/jup310.txt
	b.mu = 1.487604677404272E-01;
	_body[505] = b;

	b = Body();
	b.centerBody = jupiter;
	// b.mu = 0.1402097489313343;     // +/- 100
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/jup300.txt
	b.mu = 1.402097489313343E-01;
	b.name = "himalia";
	_body[506] = b;

	b = Body();
	b.centerBody = jupiter;
	//b.mu = G * 8.7e17;
	b.mu = 0; // jpl uses 0
	b.name = "elara";
	_body[507] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 8.7e17;
    b.mu = 0; // jpl uses 0
    b.name = "pasiphae";
    _body[508] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 8.7e17;
    b.mu = 0; // jpl uses 0
    b.name = "sinope";
    _body[509] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 8.7e17;
    b.mu = 0; // jpl uses 0
    b.name = "lysithea";
    _body[510] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 8.7e17;
    b.mu = 0; // jpl uses 0
    b.name = "carme";
    _body[511] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 8.7e17;
    b.mu = 0; // jpl uses 0
    b.name = "ananke";
    _body[512] = b;

	b = Body();
	b.centerBody = jupiter;
	b.name = "leda";
	//b.mu = G * 2e15;
	b.mu = 0; // jpl uses 0
	_body[513] = b;

    b = Body();
    b.centerBody = jupiter;
    b.name = "thebe";
    //b.mu = G * 2e15;
    b.mu = 0; // jpl uses 0
    _body[514] = b;

    b = Body();
    b.centerBody = jupiter;
    b.name = "adrastea";
    //b.mu = G * 2e15;
    b.mu = 0; // jpl uses 0
    _body[515] = b;

	b = Body();
	b.centerBody = jupiter;
	//b.mu = G * 3.6e16;
	b.mu = 0; // jpl uses 0
	b.name = "metis";
	_body[516] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 3.6e16;
    b.mu = 0; // jpl uses 0
    b.name = "callirrhoe";
    _body[517] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 3.6e16;
    b.mu = 0; // jpl uses 0
    b.name = "themisto";
    _body[518] = b;

	b = Body();
	b.centerBody = jupiter;
	//b.mu = G * 0.021e16;
	b.mu = 0; // jpl uses 0
	b.name = "megaclite";
	_body[519] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "taygete";
    _body[520] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "chaldene";
    _body[521] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "harpalyke";
    _body[522] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "kalyke";
    _body[523] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "iocaste";
    _body[524] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "erinome";
    _body[525] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "isonoe";
    _body[526] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "praxidike";
    _body[527] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "autonoe";
    _body[528] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "thyone";
    _body[529] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "hermippe";
    _body[530] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "aitne";
    _body[531] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "eurydome";
    _body[532] = b;

    b = Body();
    b.centerBody = jupiter;
    //b.mu = G * 0.021e16;
    b.mu = 0; // jpl uses 0
    b.name = "euanthe";
    _body[533] = b;

    shared_ptr<Body> saturn(new Body());
	saturn->centerBody = sun;
	saturn->bfrm.reset(new SaturnFrameRotMatrix());
	// JPL
	// b.j.reset(new vector<PrecType> {0, 0, 1.6290695e-2, 0, -9.343511e-4, 0, 8.959557e-5, 0, -1e-5});
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	/*
	saturn->j.reset(new vector<PrecType> {
        0, 0, 1.629069546359586E-02, 0,
        -9.343510777884500E-04, 0,
        8.959557103488101E-05, 0,
        -1.000000000000000E-05
    });
    */
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
	saturn->j.reset(new vector<PrecType> {
	    0, 0, 1.629111964057611E-02, 1.514844984134440E-06,
	    -9.305945887751694E-04, 0,
	    8.872819837018759E-05, 0,
	    -1.044228535152935E-05
	});
    // Anderson & Schubert (Table 2, Column 2)
	// _J['699'] = [0, 0, 1.63038e-2, 0, -9.373e-4, 0, 8.43e-5, 0, -1e-5]
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	saturn->radius = 60330;
	//_radius['699'] = 60268
	// JPL
	// Anderson & Schubert (Table 2, Column 2)
	// _radius['699'] = 60305.7
	// b.mu = 37931207.8;   // +/- 98
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
    //saturn->mu = 3.793120665570859E+07;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
   saturn->mu = 3.793120723493890E+07;
	saturn->name = "saturn";
	saturn->id = 699;
	_body[saturn->id] = *saturn;

	b = Body();
	b.centerBody = saturn;
	// b.mu = G * 3.75e19;  // +/- 0.16
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	// b.mu = 2.503375434781544E+00;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
	b.mu = 2.503458199931431E+00;
	b.name = "mimas";
	_body[601] = b;

	b = Body();
	b.centerBody = saturn;
	// b.mu = G * 10.805e19; // +/- 0.16
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	// b.mu = 7.211233789588942E+00;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
	b.mu = 7.211185066509890E+00;
	b.name = "enceladus";
	_body[602] = b;

	b = Body();
	b.centerBody = saturn;
	// b.mu = G * 61.76e19;  // +/- 0.16
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	// b.mu = 4.120932250034921E+01;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
	b.mu = 4.120856508658532E+01;
	b.name = "tethys";
	_body[603] = b;

	b = Body();
	b.centerBody = saturn;
	// b.mu = G * 109.572e19;  // +/- 0.16
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	// b.mu = 7.311592012060677E+01;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
	b.mu = 7.311574218947423E+01;
	b.name = "dione";
	_body[604] = b;

	b = Body();
	b.centerBody = saturn;
	// b.mu = 153.94;  // +/- 0.16
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	// b.mu = 1.539439497208566E+02;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
	b.mu = 1.539419035933117E+02;
	b.name = "rhea";
	_body[605] = b;

	b = Body();
	b.centerBody = saturn;
	// b.mu = 8978.13; //  +-  0.06
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	// b.mu = 8.978138015653407E+03;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
	b.mu = 8.978137030983542E+03;
	b.name = "titan";
	_body[606] = b;

	b = Body();
	b.centerBody = saturn;
	// b.mu = G * 1.08e19;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	//b.mu = 3.715249700201681E-01;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
	b.mu = 3.712085754472412E-01;
	b.name = "hyperion";
	_body[607] = b;

	b = Body();
	b.centerBody = saturn;
	// b.mu = 120.51; //   +- 0.03
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	//b.mu = 1.205006955066812E+02;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
	b.mu = 1.205095752388872E+02;
	b.name = "iapetus";
	_body[608] = b;

	b = Body();
	b.centerBody = saturn;
	// b.mu = G * 0.8289e19;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	//b.mu = 5.539027499376604E-01;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
	b.mu = 5.532371285376407E-01;
	b.name = "phoebe";
	_body[609] = b;

	b = Body();
	b.centerBody = saturn;
	// b.mu = G * 0.0198e20;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
	b.mu = 1.265529923304381E-01;
	b.name = "janus";
	_body[610] = b;

    b = Body();
    b.centerBody = saturn;
    // b.mu = G * 0.0198e20;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
    b.mu =3.511993291794538E-02 ;
    b.name = "epimetheus";
    _body[611] = b;

    b = Body();
    b.centerBody = saturn;
    // b.mu = G * 0.0198e20;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
    // b.mu = 0;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat389xl.txt
    b.mu = 2.055657413000371E-04;
    b.name = "helene";
    _body[612] = b;

    b = Body();
    b.centerBody = saturn;
    // b.mu = G * 0.0198e20;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
    b.mu = 0;
    b.name = "telesto";
    _body[613] = b;

    b = Body();
    b.centerBody = saturn;
    // b.mu = G * 0.0198e20;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
    b.mu = 0;
    b.name = "calypso";
    _body[614] = b;

    b = Body();
    b.centerBody = saturn;
    // b.mu = G * 0.0198e20;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
    b.mu = 3.786366936451041E-04;
    b.name = "atlas";
    _body[615] = b;

    b = Body();
    b.centerBody = saturn;
    // b.mu = G * 0.0198e20;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
    b.mu =1.069074283364336E-02 ;
    b.name = "prometheus";
    _body[616] = b;

    b = Body();
    b.centerBody = saturn;
    // b.mu = G * 0.0198e20;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/sat382.txt
    b.mu = 9.167191181381035E-03;
    b.name = "pandora";
    _body[617] = b;

    b = Body();
    b.centerBody = saturn;
    b.mu = 0;
    b.name = "pan";
    _body[618] = b;

    b = Body();
    b.centerBody = saturn;
    b.mu = 0;
    b.name = "ymir";
    _body[619] = b;

    b = Body();
    b.centerBody = saturn;
    b.mu = 0;
    b.name = "paaliaq";
    _body[620] = b;

    b = Body();
    b.centerBody = saturn;
    b.mu = 0;
    b.name = "tarvos";
    _body[621] = b;

    b = Body();
    b.centerBody = saturn;
    b.mu = 0;
    b.name = "ijiraq";
    _body[622] = b;

    b = Body();
    b.centerBody = saturn;
    b.mu = 0;
    b.name = "suttungr";
    _body[623] = b;

    b = Body();
    b.centerBody = saturn;
    b.mu = 0;
    b.name = "kiviuq";
    _body[624] = b;

    b = Body();
    b.centerBody = saturn;
    b.mu = 0;
    b.name = "mundilfari";
    _body[625] = b;

    b = Body();
    b.centerBody = saturn;
    b.mu = 0;
    b.name = "albiorix";
    _body[626] = b;

    b = Body();
    b.centerBody = saturn;
    b.mu = 0;
    b.name = "skathi";
    _body[627] = b;

    b = Body();
    b.centerBody = saturn;
    b.mu = 0;
    b.name = "erriapus";
    _body[628] = b;

    shared_ptr<Body> uranus(new Body());
	uranus->centerBody = sun;
	uranus->bfrm.reset(new UranusFrameRotMatrix());
	// JPL
//	b.j.reset(new vector<PrecType> {0, 0, 3.510685e-3, 0,-3.4166397e-5});
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura095.txt
    // b.j.reset(new vector<PrecType> {0, 0, 3.341003943875955E-03, 0, -3.088010538712617E-05}
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    uranus->j.reset(
        new vector<PrecType> {0, 0, 3.510685384697763E-03, 0, -3.416639735448987E-05}
    );
	// b.mu = 5793966;     // +/- 23
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    uranus->mu = 5.793951322279009E+06;
	uranus->name = "uranus";
	// JPL equatorial radius
	// b.radius = 25559;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura095.txt
    // b.radius = 26200
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    uranus->radius = 25559;
    uranus->id = 799;
	_body[uranus->id] = *uranus;

	b = Body();
	b.centerBody = uranus;
	//b.mu = G * 13.53e20; // +- 1.20
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
	b.mu = 8.346344431770477E+01;
	b.name = "ariel";
	_body[701] = b;

	b = Body();
	b.centerBody = uranus;
	// b.mu = G * 11.72; // +- 1.35
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
	b.mu = 8.509338094489388E+01;
	b.name = "umbriel";
	_body[702] = b;

	b = Body();
	b.centerBody = uranus;
	// b.mu = G * 35.27e20;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
	b.mu = 2.269437003741248E+02;
	b.name = "titania";
	_body[703] = b;

	b = Body();
	b.centerBody = uranus;
	// b.mu = G * 30.14e20; // +- 0.75;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
	b.mu = 2.053234302535623E+02;
	b.name = "oberon";
	_body[704] = b;

	b = Body();
	b.centerBody = uranus;
	// b.mu = G * 0.659e20; // +- 0.075
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
	b.mu = 4.319516899232100E+00;
	b.name = "miranda";
	_body[705] = b;
/*
    b = Body();
    b.centerBody = uranus;
    b.mu = 0;
    b.name = "cordelia";
    _body[706] = b;

    b = Body();
    b.centerBody = uranus;
    b.mu = 0;
    b.name = "ophelia";
    _body[707] = b;

    b = Body();
    b.centerBody = uranus;
    b.mu = 0;
    b.name = "bianca";
    _body[708] = b;

    b = Body();
    b.centerBody = uranus;
    b.mu = 0;
    b.name = "cressida";
    _body[709] = b;

    b = Body();
    b.centerBody = uranus;
    b.mu = 0;
    b.name = "desdemona";
    _body[710] = b;

    b = Body();
    b.centerBody = uranus;
    b.mu = 0;
    b.name = "juliet";
    _body[711] = b;

    b = Body();
    b.centerBody = uranus;
    b.mu = 0;
    b.name = "portia";
    _body[712] = b;

    b = Body();
    b.centerBody = uranus;
    b.mu = 0;
    b.name = "rosalind";
    _body[713] = b;

    b = Body();
    b.centerBody = uranus;
    b.mu = 0;
    b.name = "belinda";
    _body[714] = b;
*/
    b = Body();
    b.centerBody = uranus;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    b.mu = 0;
    b.name = "puck";
    _body[715] = b;

    b = Body();
    b.centerBody = uranus;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    b.mu = 0;
    b.name = "caliban";
    _body[716] = b;

    b = Body();
    b.centerBody = uranus;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    b.mu = 0;
    b.name = "sycorax";
    _body[717] = b;

    b = Body();
    b.centerBody = uranus;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    b.mu = 0;
    b.name = "prospero";
    _body[718] = b;

    b = Body();
    b.centerBody = uranus;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    b.mu = 0;
    b.name = "setebos";
    _body[719] = b;

    b = Body();
    b.centerBody = uranus;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    b.mu = 0;
    b.name = "stephano";
    _body[720] = b;

    b = Body();
    b.centerBody = uranus;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    b.mu = 0;
    b.name = "trinculo";
    _body[721] = b;

    b = Body();
    b.centerBody = uranus;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    b.mu = 0;
    b.name = "francisco";
    _body[722] = b;

    b = Body();
    b.centerBody = uranus;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    b.mu = 0;
    b.name = "margaret";
    _body[723] = b;

    b = Body();
    b.centerBody = uranus;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/ura112.txt
    b.mu = 0;
    b.name = "ferdinand";
    _body[724] = b;

	shared_ptr<Body> neptune(new Body());
    neptune->bfrm.reset(new NeptuneFrameRotMatrix());
	neptune->centerBody = sun;
    //    b.j.reset(new vector<PrecType> {0, 0, 3.4084285e-3, 0, -3.33989e-5});
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/nep088.txt
    neptune->j.reset(
        new vector<PrecType> {
            0, 0, 3.408428530717952E-03,
            0, -3.339891759006578E-05
        }
    );
	// b.mu = 6835107;     // +/- 15
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/nep088.txt
    neptune->mu = 6.835099502439672E+06;
	neptune->name = "neptune";
	// JPL equatorial radius
	// b.radius = 24766; // +-15
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/nep088.txt
    neptune->radius = 25225;
    neptune->id = 899;
	_body[neptune->id] = *neptune;

	b = Body();
	b.centerBody = neptune;
	// b.mu = G * 214.7e20; // +/- 0.7
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/nep088.txt
	b.mu = 1.427598140725034E+03;
	b.name = "triton";
	_body[801] = b;

    b = Body();
    b.centerBody = neptune;
    // b.mu = G * 2.7e19; // wikipedia moons of neptune table
    b.mu = 0; // jpl uses 0 mass
    b.name = "nereid";
    _body[802] = b;

    b = Body();
    b.centerBody = neptune;
   //  b.mu = G * 2.10e18; // wikipedia moons of neptune table
    b.mu = 0; // jpl uses 0 mass
    b.name = "despina";
    _body[805] = b;

    b = Body();
    b.centerBody = neptune;
    // b.mu = G * 3.75e18; // wikipedia moons of neptune table
    b.mu = 0; // jpl uses 0 mass
    b.name = "galatea";
    _body[806] = b;

    b = Body();
    b.centerBody = neptune;
    // b.mu = G * 4.95e18; // wikipedia moons of neptune table
    b.mu = 0; // jpl uses 0 mass
    b.name = "larissa";
    _body[807] = b;

    b = Body();
    b.centerBody = neptune;
    // b.mu = G * 5.035e19; // wikipedia moons of neptune table
    b.mu = 0; // JPL uses 0 mass
    b.name = "proteus";
    _body[808] = b;

	shared_ptr<Body> pluto(new Body());
	b.centerBody = sun;
	// b.mu = 872.4;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/plu055l.txt
    pluto->mu = 869.3390780381926;
	pluto->name = "pluto";
	pluto->id = 999;
	_body[pluto->id] = *pluto;

	b = Body();
	b.centerBody = pluto;
	// b.mu = 102.271;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/plu055l.txt
    b.mu = 106.2509269522026;
	b.name = "charon";
	_body[901] = b;

	b = Body();
	b.centerBody = pluto;
	// ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/plu055l.txt
	b.mu = 2.150552267969335E-03;
	b.name = "nix";
	_body[902] = b;

    b = Body();
    b.centerBody = pluto;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/plu055l.txt
    b.mu = 3.663917107480563E-03;
    b.name = "hydra";
    _body[903] = b;

    b = Body();
    b.centerBody = pluto;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/plu055l.txt
    b.mu = 4.540734312735987E-04;
    b.name = "kerberos";
    _body[904] = b;

    b = Body();
    b.centerBody = pluto;
    // ftp://ssd.jpl.nasa.gov/pub/eph/satellites/nio/LINUX_PC/plu055l.txt
    b.mu = 2.000000000000000E-20;
    b.name = "styx";
    _body[905] = b;

    b = Body();
    b.centerBody = sun;
    b.mu = 63.2;
    b.name = "ceres";
    _body[2000001] = b;

	vector<PrecType> f {0, 0, 3.0, 0.5, 2.5, 0.375, 0.875, 0.0625, 0.5625};
	for (auto& p : _body) {
	    auto& b = p.second;
		_nameToID[b.name] = p.first;
		b.id = p.first;
		if (b.j && b.j->size() >= 3) {
		    auto n = b.j->size();
		    b.c.resize(n);
		    for (uint i=0; i<n ; ++i) {
		        b.c[i] = f[i] * b.j->operator [](i)*b.mu;
		    }
		}
	}
}

}
