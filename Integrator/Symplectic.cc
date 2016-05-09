/*

 *      Author: dmehring
 */

#include <Integrator/Symplectic.h>

#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>
#include <IO/IO.h>
#include <Physics/NBodySystem.h>
#include <Integrator/PositionPublisher.h>

#include <iomanip>

using namespace std;
using namespace boost::numeric::odeint;

namespace kepler {

Symplectic::Symplectic(
    NBodySystem* system, AccelerationCalculator* accCalc,
    TimeStepManager *const &tsm,
    const IntegrationEnder *const& ie
) : Integrator(system, accCalc, tsm, ie),
    _odeSystem(
        make_pair(
            coord(system->getMasses() ) , momentum(_accCalc, system->getMasses())
     )
    ) {
           auto n = system->getNumberOfBodies();
           _ai.resize(n);
           _af.resize(n);
        Vvector p(system->getNumberOfBodies());
        Vvector q(system->getNumberOfBodies());
        auto bodies = system->getBodies();
        auto piter = begin(p);
        auto qiter = begin(q);
        for (const auto& body: bodies) {
            *piter = body.mu/G*body.v;
            *qiter = body.x;
            ++piter;
            ++qiter;
        }
        _inState = make_pair(q, p);
        _outState = _inState;
}

Symplectic::~Symplectic() {}

void Symplectic::integrate() {
    _accCalc->setTime(_startTime, SECOND);
    _accCalc->compute(_ai, &_x);
    _odeSystem.second._a = _ai;
    _tsmd.aFinal = &_odeSystem.second._a;
     _tsmd.aInitial = &_ai;
     cout << "initial " << _tsmd.aInitial << " final " << _tsmd.aFinal << endl;
     _tsmd.currentTime = _currentTime;
     _tsmd.currentDeltaT = _deltaT;
     auto updateStep = _pp ? _pp->getNSteps() : 0;
     while(! _ie->end(_ied)) {
        _deltaT = _tsm->getDeltaT();
        _ie->setNextDeltaT(_deltaT, _ied);
        _step();
        _accCalc->compute(*_tsmd.aFinal, &_outState.first);
        _tsct = _tsm->modify(_tsmd);
        if (_tsct == TimeStepManager::DECREASE) {
            _step();
            cout << setprecision(16) << "reset time " << _currentTime << " dt " << _deltaT << endl;
        }
        else if (_tsct == TimeStepManager::INCREASE) {
            cout << "Increased dt to " << _tsm->getDeltaT() << endl;
        }
        _inState = _outState;
        _elapsedT += _deltaT;
        auto xdiff = _inState.first[1][0] -  _inState.first[2][0];
        auto ydiff = _inState.first[1][1] -  _inState.first[2][1];
        cout  << fixed << setw(10) << setprecision(0) << _elapsedT
                    << " " << setw(12) << setprecision(0) << sqrt(xdiff*xdiff + ydiff*ydiff)
                    << " " << setw(8) << setprecision(3) <<  _deltaT << endl;

        _ai = *_tsmd.aFinal;
        if (_pp && _nsteps % updateStep == 0) {
            _pp->setX(_outState.first);
        }
        ++_nsteps;
    }
    if (_pp) {
        _pp->end();
    }
    _x = _outState.first;
    auto m = _system->getMasses();
    auto miter = begin(m);
    auto piter = begin(_outState.second);
    for (auto& v: _v) {
        v = *piter/(*miter);
        ++miter;
        ++piter;
    }
}

}


