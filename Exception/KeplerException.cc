//# KeplerException.cc
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
