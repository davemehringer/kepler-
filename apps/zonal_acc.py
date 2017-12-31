# zonal_acc.py
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
from sympy import *
from sympy.abc import R, r, x, y, z
# mu = GM
mu = symbols("mu")
# potential enegy
u = {}
# acceleration components
ax = {}
ay = {}
az = {}
# zonal harmonic coefficients
J = {}
for i in range(2, 12):
    J[i] = symbols("J" + str(i))

mumu = 3.8e7
RR = 60300
rr = 300000
zz = 1000
yy = 150000
xx = sqrt(rr*rr - yy*yy - zz*zz)

for i in range(2, 12):
    u[i] = mu*R**i/r * J[i]*legendre(i, z/r)/r**i
    u[i] = u[i].subs(r, (x**2 + y**2 + z**2)**0.5)
    ax[i] = -diff(u[i], x)
    ay[i] = -diff(u[i], y)
    az[i] = -diff(u[i], z)
    print "ax",i," = ", factor(ax[i].subs((x**2 + y**2 + z**2)**0.5, r))
    print "ay",i," = ", factor(ay[i].subs((x**2 + y**2 + z**2)**0.5, r))
    print "az",i," = ", factor(az[i].subs((x**2 + y**2 + z**2)**0.5, r))

axtot = 0
aytot = 0
aztot = 0
for i in range(2, 11):
    jj = 250*i**6
    subs = { J[i]: jj, mu: mumu, R: RR, x: xx, y: yy, z: zz }
    axax = ax[i].evalf(subs=subs)
    ayay = ay[i].evalf(subs=subs)
    azaz = az[i].evalf(subs=subs)
    print "ax" + str(i), axax
    print "ay" + str(i), ayay
    print "az" + str(i), azaz
    axtot += axax
    aytot += ayay
    aztot += azaz

print("axtot %18.17f" % axtot)
print("aytot %18.17f" % aytot)
print("aztot %18.17f" % aztot)

