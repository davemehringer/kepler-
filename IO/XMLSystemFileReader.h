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
