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

