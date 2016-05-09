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

