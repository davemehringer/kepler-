/*
 * KeplerException.cc
 *
 *  Created on: Sep 3, 2015
 *      Author: dmehring
 */

#include "KeplerException.h"


namespace kepler {

KeplerException::KeplerException() : _message() {}

KeplerException::KeplerException(const string& msg)
	: _message(msg) {}

KeplerException::~KeplerException() throw() {}

string KeplerException::getMsg() const {
	return _message;
}

}
