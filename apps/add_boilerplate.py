#! /usr/bin/env python
#
# add_boilerplate.py
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

import os

bp = file("doc/boilerplate.txt")
lines = bp.readlines()[2:]
bp.close()
boilerplate_c = ""
boilerplate_p = ""
prefix_c = "//# "
prefix_p = "# "
for line in lines:
    boilerplate_p += prefix_p + line
    boilerplate_c += prefix_c + line

cw = "Copyright (C) 2016 David Mehringer"
kk = os.walk(".")
for (dir, dirs, files) in kk:
    for f in files:
        if f.endswith(".cc") or f.endswith(".h") or f.endswith(".py"):
            myfile = dir + '/' + f
            outfile = dir + '/' + f + '.tmp'
            if f.endswith("py"):
                mybp = boilerplate_p
                prefix = prefix_p
            else:
                mybp = boilerplate_c
                prefix = prefix_c
            with open(outfile, 'w') as fout, open(myfile) as fin:
                fout.write(prefix + f + "\n")
                fout.write(prefix + cw + "\n")
                fout.write(mybp)
                for line in fin:
                    fout.write(line)
            os.rename(outfile, myfile)
