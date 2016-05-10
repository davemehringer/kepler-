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

for i in range(2, 12):
    u[i] = mu*R**i/r * J[i]*legendre(i, z/r)/r**i
    u[i] = u[i].subs(r, (x**2 + y**2 + z**2)**0.5)
    ax[i] = -diff(u[i], x)
    ay[i] = -diff(u[i], y)
    az[i] = -diff(u[i], z)
    print "ax",i," = ", factor(ax[i].subs((x**2 + y**2 + z**2)**0.5, r))
    print "ay",i," = ", factor(ay[i].subs((x**2 + y**2 + z**2)**0.5, r))
    print "az",i," = ", factor(az[i].subs((x**2 + y**2 + z**2)**0.5, r))

