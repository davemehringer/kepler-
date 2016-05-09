/*
 * VectorTester.cc
 *
 *  Created on: Sep 30, 2015
 *      Author: dmehring
 */

#include "VectorTester.h"

#include <iostream>

namespace kepler {

VectorTester::VectorTester(int size) : _v(size) {
    fill(_v.begin(), _v.end(), 0);
}

VectorTester::~VectorTester() {}

void VectorTester::populate(int startIndex, int endIndex) {
    for (int i=startIndex; i<endIndex; ++i) {
        //_mutex.lock();
        _v[i] = i;
       // _mutex.unlock();
    }
}


}
