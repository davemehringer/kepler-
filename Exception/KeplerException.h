/*
 * KeplerException.h
 *
 *  Created on: Sep 3, 2015
 *      Author: dmehring
 */

#ifndef KEPLER_KEPLEREXCEPTION_H_
#define KEPLER_KEPLEREXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

namespace kepler {

class KeplerException : public exception {

public:
	KeplerException();

	KeplerException(const string& msg);

	~KeplerException() throw();

	string getMsg() const;

private:

	string _message;

};

}

#endif
