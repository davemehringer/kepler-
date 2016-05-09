/*
 * VectorTester.h
 *
 *  Created on: Sep 30, 2015
 *      Author: dmehring
 */

#ifndef SRC_VECTORTESTER_H_
#define SRC_VECTORTESTER_H_

#include <mutex>
#include <vector>

using namespace std;

namespace kepler {

// for testing threads
class VectorTester {

public:

    VectorTester(int size);
    virtual ~VectorTester();

    const vector<int>& getVector() const { return _v; }

    void populate(int startIndex, int endIndex);

private:
    vector<int> _v;
    mutex _mutex;
};

}

#endif
