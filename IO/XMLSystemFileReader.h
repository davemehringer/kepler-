//# XMLSystemFileReader.h
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
 * XMLSystemFileReader.h
 *
 *  Created on: Mar 26, 2016
 *      Author: dmehring
 */

#ifndef SRC_IO_XMLSYSTEMFILEREADER_H_
#define SRC_IO_XMLSYSTEMFILEREADER_H_

#include <Physics/NBodySystem.h>

#include <xercesc/dom/DOM.hpp>

#include <string>
#include <vector>

using namespace std;
using namespace xercesc;

namespace kepler {

class XMLSystemFileReader {

public:

    XMLSystemFileReader() = delete;

    XMLSystemFileReader(const string& filename);

    shared_ptr<const NBodySystem> getSystem() const { return _system; }

private:

    shared_ptr<const NBodySystem> _system;

    static double _getDoubleValuedAttr(
        const DOMNamedNodeMap *const atts, const string& attrName
    );

    static string _getStringValuedAttr(
        const DOMNamedNodeMap *const atts, const string& attrName
    );

};

}



#endif
