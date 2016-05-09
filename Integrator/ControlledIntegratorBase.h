/*

 *      Author: dmehring
 */

#ifndef KEPLER_CONTROLLEDINTEGRATORBASE_H
#define KEPLER_CONTROLLEDINTEGRATORBASE_H

#include <Integrator/Integrator.h>
#include <IO/IO.h>
#include <OS/Timer.h>

#include <AccelerationCalculator/AccelerationCalculator.h>

#include <boost/numeric/odeint.hpp>

#include <Math/Vector.h>

using namespace boost::numeric::odeint;

namespace kepler {

using container_type = Vvector;

class ControlledIntegratorBase: public Integrator {
public:

    using State = vector<PrecType>;

    ControlledIntegratorBase() = delete;

    ControlledIntegratorBase(
        NBodySystem* system, AccelerationCalculator* accCalc,
        TimeStepManager *const &tsm,
        const IntegrationEnder *const& ie
    );

    virtual ~ControlledIntegratorBase();

    virtual void integrate();

protected:

    Vvector _ai, _af;

    virtual void _step() = 0;

    struct coord {

        void operator()( const State &x , State &dxdt , PrecType t ) const {
            _populateFromFirst(_xs, x);
            _populateFirstFromSecond(dxdt, x);
            _aCalc->compute(_as, &_xs);
            _populateSecond(dxdt, _as);
        };
    };

    State _inState, _outState;
    static AccelerationCalculator* _aCalc;
    static Vvector _xs, _as;

    // populate the  Vvector using the first half of the state vector
   inline  static void _populateFromFirst(Vvector& x, const State& state) {
       auto iter = begin(state);
       for (auto& xx: x) {
           for (auto j=0; j<3; ++j, ++iter) {
               xx[j] = *iter;
           }
       }
   }

   // populate the first half the state using the  Vvector
  inline  static void _populateFirst(State& state, const Vvector& x) {
      auto iter = begin(state);
      for (const auto& xx: x) {
          for (auto j=0; j<3; ++j, ++iter) {
              *iter = xx[j];
          }
      }
  }

   // populate the Vvector using the second half of the state vector
   inline  static void _populateFromSecond(Vvector& v, const State& state) {
       static const auto n = state.size()/2;
       auto iter = begin(state);
       advance(iter, n);
       for (auto& vv: v) {
           for (auto j=0; j<3; ++j, ++iter) {
               vv[j] = *iter;
           }
       }
   }

   // populate the second half of the state using the Vvector
   inline  static void _populateSecond(State& state, const Vvector& v) {
       static const auto n = state.size()/2;
       auto iter = begin(state);
       advance(iter, n);
       for (const auto& vv: v) {
           for (auto j=0; j<3; ++j, ++iter) {
               *iter = vv[j];
           }
       }
   }

   // poputate the first half of x with the second half of v
   inline static void _populateFirstFromSecond(State& x, const State& v) {
       static const auto n = x.size()/2;
       auto xiter = begin(x);
       auto viter = begin(v);
       advance(viter, n);
       auto vend = end(v);
       for ( ; viter!=vend; ++viter, ++xiter) {
           *xiter = *viter;
       }
   }

};

}

#endif
