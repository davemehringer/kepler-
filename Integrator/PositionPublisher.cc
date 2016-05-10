//# PositionPublisher.cc
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
 * PositionPublisher.cc
 *
 *  Created on: Oct 9, 2015
 *      Author: dmehring
 */

#include <Integrator/PositionPublisher.h>

#include <Exception/KeplerException.h>
#include <IO/IO.h>
#include <Math/Vector.h>

#include <fcntl.h>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;

namespace kepler {

PositionPublisher::PositionPublisher(const vector<Body>& bodies) {
    cout << "open" << endl;
    _writefd = open("/tmp/fifo_kepler", O_WRONLY);
    _canWrite = _writefd >= 0;
    cout << "can write " << _canWrite << endl;
    if (! _canWrite) {
        cerr << "Cannot open /tmp/fifo_kepler for writing: " << strerror(errno) << endl;
        return;
    }
    if (! bodies.empty()) {
        ostringstream oss;
        for (const auto& b : bodies) {
            oss << b.name << " ";
        }
        oss << endl;
        auto str = oss.str();

        if (write(_writefd, str.c_str(), str.size()) < 0) {
            _canWrite = false;
        }
    }
}

PositionPublisher::~PositionPublisher() {
    if (! _done) {
        end();
    }
}

void PositionPublisher::setX(const Vvector& x) {
    ostringstream oss;
    for (const auto& v: x) {
        oss << v[0] << " " << v[1] << " " << v[2] << " ";
    }
    oss << endl;
    auto s = oss.str();
    if (write(_writefd, s.c_str(), s.size()) < 0) {
        _canWrite = false;
    }
}

void PositionPublisher::end() {
    ostringstream oss;
    oss << "end" << endl;
    auto s = oss.str();
    write(_writefd, s.c_str(), s.size());
    close(_writefd);
    _done = true;
}








        /* send 7 characters in the string, including end-of-string */
/*
        cout << "About to send a message: " << endl;
                write(pipefds[1], "Hi Mom!", 7);

        close(pipefds[1]);
        exit(0);
    }
        else   */     /* parent : receives message from child */
   // {
        /* close unused end of pipe */
        /* because this process only needs to read */
       // close(pipefds[1]);

        /* read from the pipe */
     //           read(pipefds[0], instring, 7);
       // cout << "Just received a message that says: " << instring << endl;

       // close(pipefds[0]);
       // exit(0);
   // }
//}
    /*
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    */
    //socket.bind ("tcp://*:5555");
/*
        //  Send data
        zmq::message_t reply (5);
        memcpy ((void *) reply.data (), "World", 5);
        socket.send (reply);
        */
}


