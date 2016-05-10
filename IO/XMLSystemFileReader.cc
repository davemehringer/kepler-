//# XMLSystemFileReader.cc
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
#include <IO/XMLSystemFileReader.h>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <Exception/KeplerException.h>
#include <Math/Vector.h>
#include <Physics/Body.h>
#include <Physics/StateVectorsOrbitalElements.h>
#include <SolarSystem/SSObjects.h>

#include <iostream>

//using namespace std;
//using namespace xercesc;

namespace kepler {

XMLSystemFileReader::XMLSystemFileReader(const string& filename) {
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        string msg(message);
        XMLString::release(&message);
        throw KeplerException(msg);
    }

    unique_ptr<XercesDOMParser> parser( new XercesDOMParser());
    parser->setValidationScheme(XercesDOMParser::Val_Always);
    parser->setDoNamespaces(true);    // optional
    unique_ptr<ErrorHandler> errHandler( (ErrorHandler*) new HandlerBase());
    parser->setErrorHandler(errHandler.get());
    try {
        parser->parse(filename.c_str());
     }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        string msg(message);
        XMLString::release(&message);
        throw KeplerException(msg);
    }
    catch (const DOMException& toCatch) {
        char* message = XMLString::transcode(toCatch.msg);
        string msg(message);
        XMLString::release(&message);
        throw KeplerException(msg);
    }
    catch (...) {
        throw KeplerException("Unexpected Exception") ;
    }
    auto doc = parser->getDocument();
    auto rootNode = doc->getElementsByTagName(XMLString::transcode("cbody"))->item(0);
    auto atts = rootNode->getAttributes();
    auto name = _getStringValuedAttr(atts, "name");
    auto mu = _getDoubleValuedAttr(atts, "mass");
    auto solarMu = SSObjects::createBody("sun").mu;
    mu *= solarMu;
    shared_ptr<Body> centerBody(new Body());
    centerBody->name = name;
    centerBody->mu = mu;
    centerBody->x = {0, 0, 0};
    centerBody->v = {0, 0, 0};
    vector<Body> bodies;
    auto startTime = _getDoubleValuedAttr(atts, "time");
    bodies.push_back(*centerBody);
    auto xmlBodies = rootNode->getChildNodes();
    auto n = xmlBodies->getLength();
    auto id = 1;
    for (unsigned int j=0; j<n; ++j) {
        auto body = xmlBodies->item(j);
        auto x = body->getNodeType();
        if (x == DOMNode::ELEMENT_NODE) {
            atts = body->getAttributes();
            mu = _getDoubleValuedAttr(atts, "mass") * solarMu;
            name = _getStringValuedAttr(atts, "name");
            Elements elements;
            elements.a = _getDoubleValuedAttr(atts, "a");
            elements.e = _getDoubleValuedAttr(atts, "e");
            elements.i = _getDoubleValuedAttr(atts, "i");
            elements.peri = _getDoubleValuedAttr(atts, "peri");
            elements.node = _getDoubleValuedAttr(atts, "node");
            elements.mAnomaly = _getDoubleValuedAttr(atts, "mAnomaly");
            auto vectors = elementsToVectors(centerBody->mu, mu, elements);
            Body body;
            body.mu = mu;
            body.id = id;
            body.name = name;
            body.centerBody = centerBody;
            body.origin = SYSTEM_BARYCENTER;
            body.x = vectors.first;
            body.v = vectors.second;
            bodies.push_back(body);
            ++id;
        }
    }
    _system.reset(new NBodySystem(bodies, startTime, SECOND));
}

double XMLSystemFileReader::_getDoubleValuedAttr(
    const DOMNamedNodeMap *const atts, const string& attrName
) {
    return atof(
        XMLString::transcode(
            atts->getNamedItem(
                XMLString::transcode(attrName.c_str())
            )->getNodeValue()
        )
    );
}


string XMLSystemFileReader::_getStringValuedAttr(
    const DOMNamedNodeMap *const atts, const string& attrName
) {
    return string(
        XMLString::transcode(
            atts->getNamedItem(
                XMLString::transcode(attrName.c_str())
            )->getNodeValue()
        )
    );
}

}
