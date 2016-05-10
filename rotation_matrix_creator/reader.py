# reader.py
# Copyright (C) 2016 David Mehringer
# 
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
# 
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
# 
#     You should have received a copy of the GNU General Public License
#     along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
import numpy

def read(filename):
    f = open(filename)
    lines = f.readlines()
    f.close()
    begin = False
    x = {}
    number = 0
    for line in lines:
        line = line.rstrip()
        if line == "$$SOE":
            begin = True
            continue
        elif line == "$$EOE":
            break
        if not begin:
            continue
        if number == 0:
            key = float(line.split()[0])
        elif number == 1:
            x[key] = numpy.array(line.split(), float)
        number += 1
        if number == 4:
            number = 0
    return x

def process():
    ae = read("triton_sv_eclip.txt")
    ab = read("triton_sv_body.txt")

    be = read("nereid_sv_eclip.txt")
    bb = read("nereid_sv_body.txt")

    ce = read("naiad_sv_eclip.txt")
    cb = read("naiad_sv_body.txt")

    eclip = numpy.zeros([3,3], float)
    body = numpy.zeros([3,3], float)
    mat = {}
    for k in ae.keys():
        eclip[0:] = ae[k]
        eclip[1:] = be[k]
        eclip[2:] = ce[k]
        body[0:] = ab[k]
        body[1:] = bb[k]
        body[2:] = cb[k]
        mat[k] = numpy.linalg.solve(eclip, body)
    return mat

def output():
    d = process()
    keys = d.keys()
    keys.sort()
    for key in keys:
        m = d[key]
        print "    _data[" + str(key) + "] = RotMatrix3x3("
        print "        array<double, 9> {"
        print "           " + str(m[0,0]) + ", " + str(m[0,1]) + ", " + str(m[0,2]) + ","
        print "           " + str(m[1,0]) + ", " + str(m[1,1]) + ", " + str(m[1,2]) + ","
        print "           " + str(m[2,0]) + ", " + str(m[2,1]) + ", " + str(m[2,2])
        print "        }"
        print "    );"

