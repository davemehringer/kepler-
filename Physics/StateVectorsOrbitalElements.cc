#include <Physics/StateVectorsOrbitalElements.h>

#include <Exception/KeplerException.h>
#include <Math/Constants.h>
#include <Math/Math.h>
#include <Math/Vector.h>

#include <cmath>

using namespace std;

namespace kepler {

pair<Vector, Vector> elementsToVectors(
    PrecType muPrimary, PrecType muSecondary, const Elements& elements
) {
    auto a = elements.a * KM_PER_AU;
    auto i = elements.i * RAD_PER_DEG;
    auto ma = elements.mAnomaly * RAD_PER_DEG;
    auto node = elements.node *RAD_PER_DEG ;
    auto peri = elements.peri *RAD_PER_DEG ;
    auto mu = muPrimary + muSecondary;
    auto E = ma;
    const PrecType epsilon = 1e-16;
    PrecType Eprev = 1e10;
    while (abs(E - Eprev) > epsilon) {
        Eprev = E;
        E = elements.e*sin(E) + ma;
    }
    auto cp = cos(peri);
    auto cn = cos(node);
    auto ci = cos(i);
    auto sp = sin(peri);
    auto sn = sin(node);
    auto si = sin(i);
    Vector P {
        cp*cn - sp*sn*ci,
        cp*sn + sp*cn*ci,
        sp*si
    };
    Vector Q {
         -sp*cn - cp*ci*sn,
        -sp*sn + cp*ci*cn,
        si*cp
    };
    auto sE = sin(E);
    auto cE = cos(E);
    auto sqrt1_e2 = sqrt(1 - elements.e*elements.e);
    Vector r = a*(cE - elements.e)*P
        + a*sqrt1_e2*sE*Q;
    Vector v = 1.0/(1-elements.e*cE)*sqrt(mu/a)
        *(-sE*P + sqrt1_e2*cE*Q);
    return pair<Vector, Vector>(r, v);
}

Elements vectorsToElements(
    PrecType muPrimary, PrecType muSecondary, const Vector& x, const Vector& v
) {
    auto h = x.cross(v);
    auto mu = muPrimary + muSecondary;
    auto xmag = x.mag();
    Vector evec = v.cross(h)/mu - x.unit();
    auto e = evec.mag();
    auto a = h.mag2()/(mu*(1 - e*e));
    auto twopi = 2*M_PI;
    /*
        PrecType period = 0;

    if (a > 0) {
        period = twopi * a*sqrt(a/mu);
    }
    */
    auto i = acos(h[2]/h.mag());
    Vector k(0, 0, 1);
    auto n = k.cross(h);
    auto nmag = n.mag();
    PrecType node = nmag > 0 ? acos(n[0]/nmag) : 0;
    if (n[1] < 0) {
        node = twopi - node;
    }
    PrecType peri = 0;
    PrecType epsilon = 1e-8;
    if (e > 0 && nmag > 0) {
        // auto myfac = n.dot(evec)/nmag/e;
        auto myfac = n.unit().dot(evec.unit());
        if (myfac > 1 && myfac < 1 + epsilon) {
            peri = 0;
        }
        else if (myfac < -1 and myfac > -1 - epsilon) {
            peri = -M_PI;
        }
        else {
            peri = acos(myfac);
        }
    }
    if (evec[2] < 0) {
        peri = twopi - peri;
    }
    PrecType trueAnomaly = 0;
    if (e == 0) {
        trueAnomaly = acos(x[0]/xmag);
    }
    else {
        auto myfac = evec.dot(x)/e/xmag;
        if (myfac > 1 && myfac < 1+epsilon) {
            trueAnomaly = 0;
        }
        else if (myfac < -1 && myfac > -1-epsilon) {
            trueAnomaly = M_PI;
        }
        else {
            trueAnomaly = acos(myfac);
        }
    }
    if (x.dot(v) < 0) {
        trueAnomaly = twopi - trueAnomaly;
    }
    if (nmag == 0 && e > 0) {
        auto actualAngle = acos(x[0]/xmag);
        if (x[1] < 0) {
            actualAngle = twopi - actualAngle;
        }
        peri = actualAngle - trueAnomaly;
    }
    auto cosTA = cos(trueAnomaly);
    auto cosE = (e + cosTA)/(1 + e*cosTA);
    Elements el;
    if (abs(cosE) > 1) {
        el.a = a;
        el.e = e;
        el.i = i * DEG_PER_RAD;
        el.peri = peri * DEG_PER_RAD;
        el.node = numeric_limits<PrecType>::quiet_NaN();
        el.mAnomaly = numeric_limits<PrecType>::quiet_NaN();
    }
    auto E = acos(cosE);
    if (trueAnomaly > M_PI && trueAnomaly < twopi) {
        E = twopi - E;
    }
    auto ma = E - e*sin(E);
    if (std::isnan(ma)) {
        throw KeplerException("mean anomaly is NaN");
    }
    el.a = a/KM_PER_AU;
    el.e = e;
    el.i = i * DEG_PER_RAD;
    el.peri = peri * DEG_PER_RAD;
    el.node = node * DEG_PER_RAD;
    el.mAnomaly = ma * DEG_PER_RAD;
    return el;
}

bool near(const Elements& e1, const Elements& e2, PrecType epsilon) {
    return near(e1.a, e2.a, epsilon)
        && nearAbs(e1.e, e2.e, epsilon)
        && nearAbs(e1.i, e2.i, epsilon)
        && nearAbs(e1.node, e2.node, epsilon)
        && nearAbs(e1.peri, e2.peri, epsilon)
        && nearAbs(e1.mAnomaly, e2.mAnomaly, epsilon);
}

ostream& operator <<(ostream& os, const Elements& e) {
    os << "a=" << e.a << " e=" << e.e << " i=" << e.i << " peri=" << e.peri
        << " node=" << e.node << " ma=" << e.mAnomaly;
    return os;
}


}
