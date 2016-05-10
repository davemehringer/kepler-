//# kepler.cc
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
#include <AccelerationCalculator/ThreadedPairWiseAccelerationCalculator.h>
#include <AccelerationCalculator/PairWiseAccelerationCalculator.h>
#include <DistanceCalculator/ThreadedDistanceCalculator.h>
#include <DistanceCalculator/UnthreadedDistanceCalculator.h>
#include <Exception/KeplerException.h>
#include <Math/Constants.h>
#include <IntegrationEnder/ElapsedTimeIntegrationEnder.h>
#include <IntegrationEnder/NStepsIntegrationEnder.h>
#include <Integrator/ControlledIntegrator.h>
#include <Integrator/RungeKutta.h>
#include <Integrator/StepperFactory.h>
#include <Integrator/SymplecticRknSB3AMclachlan.h>
#include <Integrator/SymplecticRknSB3AM4Mclachlan.h>

#include <config/KeplerConfig.h>
#include <IO/IO.h>
#include <IO/XMLSystemFileReader.h>
#include <Physics/Body.h>
#include <Physics/NBodySystem.h>
#include <Integrator/PositionPublisher.h>
#include <SolarSystem/JPLHorizonsRetriever.h>
#include <SolarSystem/SSComparer.h>
#include <SolarSystem/SSObjects.h>
#include <TimeStepManager/AccelerationTimeStepManager.h>

#include <iomanip>
#include <iostream>
#include <set>
#include <unistd.h>

using namespace std;

using namespace kepler;

int main(int argc, char **argv) {
    try {
    Timer timer;
    timer.start();
    cout << "Kepler v" << Kepler_VERSION_MAJOR
        << "." << Kepler_VERSION_MINOR << endl;
    PrecType startTime = 0;
    string bodyStr;
    string xmlFile;
    int nsteps = 0;
    PrecType maxElapsed = 0;
    int nthreads = 1;
    int nAccThreads = 1;
    PrecType amin = 0.002;
    PrecType amax = 0.005;
    auto plotSteps = -1;
    auto pairWiseIntegrator = true;
    auto symplecticIntegrator = false;
    auto m4 = false;
    auto bs = false;
    auto karp = false;
    auto fehlberg = false;
    PrecType absError = 1e-10;
    PrecType relError = 1e-10;
    if (argc > 1) {
        for (int i=1; i<argc; i += 2) {
            string x(argv[i]);
            if (x == "-a") {
                auto minmax = split(argv[i+1],regex( ","));
                amin = atof(minmax[0].c_str());
                amax = atof(minmax[1].c_str());
            }
            else if (x == "-b") {
                bodyStr = argv[i+1];
            }
            else if (x == "-c") {
                auto err = split(argv[i+1], regex(","));
                absError = atof(err[0].c_str());
                relError = atof(err[1].c_str());
            }
            else if (x == "-e" || x == "--elapsed") {
                maxElapsed = atof(argv[i+1]);
            }
            else if (x == "-f") {
                xmlFile = argv[i+1];
            }
            else if (x == "-h" || x == "--nthreads") {
                nthreads = atoi(argv[i+1]);
            }
            else if (x == "-i" || x == "--integrator") {
                pairWiseIntegrator = string(argv[i+1]) == "p";
                symplecticIntegrator = string(argv[i+1]) == "s";
                m4 = string(argv[i+1]) == "m4";
                bs = string(argv[i+1]) == "bs";
                karp = string(argv[i+1]) == "karp";
                fehlberg = string(argv[i+1]) == "fehl";
            }
            else if (x == "-j" || x == "--nathreads") {
                nAccThreads = atoi(argv[i+1]);
            }
            else if (x == "-p") {
                plotSteps = atoi(argv[i+1]);
            }
            else if (x == "-s" || x == "--nsteps") {
                nsteps = atoi(argv[i+1]);
            }
            else if (x == "-t") {
               startTime = atof(argv[i+1]);
            }
            else {
                cerr << "Unknown option " << x << endl;
                return 1;
            }
        }
    }
    if (nsteps != 0 && maxElapsed != 0) {
        throw KeplerException("Cannot supply both the -s and -e options simultaneously");
    }
    if (! bodyStr.empty() && ! xmlFile.empty()) {
        throw KeplerException("Cannot specify both -b and -f options simultaneously");
    }
    if (nsteps == 0 && maxElapsed == 0) {
        nsteps = 200000;
    }
    vector<Body> bodies;
    if (! bodyStr.empty()) {
        auto myBodies = split(bodyStr, regex(","));
        auto sun = find(myBodies.begin(), myBodies.end(), "sun");
        if (sun != myBodies.end()) {
            myBodies.erase(sun);
        }
        JPLHorizonsRetriever jhr(JPLHorizonsRetriever::VECTORS);
        for (auto& b: myBodies) {
           jhr.add(trim(b), "sun",startTime, JPLHorizonsRetriever::ECLIPTIC);
        }
        jhr.retrieve();
        bodies = jhr.getBodies();
        bodies.insert(bodies.begin(), SSObjects::createBody("sun"));
    }
    else if (! xmlFile.empty()) {
        XMLSystemFileReader reader(xmlFile);
        auto sy = reader.getSystem();
        bodies = sy->getBodies();
        startTime = sy->getTime(DAY);
    }
    else {
        Body star0 = SSObjects::createBody("sun");
        auto star1 = star0;
        PrecType au = KM_PER_AU;
        star1.x[0] = au;
        PrecType period = M_PI*sqrt(2/star0.mu*au*au*au);
        star1.v[1] = 2*M_PI*au/period;
        bodies.resize(2);
        bodies[0] = star0;
        bodies[1] = star1;
    }
    NBodySystem system(bodies, startTime, DAY );
    unique_ptr<IntegrationEnder> ie;
    if (nsteps > 0 ) {
        ie.reset(new NStepsIntegrationEnder(nsteps));
    }
    else {
        ie.reset(new ElapsedTimeIntegrationEnder(maxElapsed));
    }
    unique_ptr<DistanceCalculator> dc;
    unique_ptr<AccelerationCalculator> ac;
    unique_ptr<Integrator> integrator;
    unique_ptr<TimeStepManager> tsm;

        if (nthreads == 1) {
            dc.reset(new UnthreadedDistanceCalculator());
        }
        else {
            dc.reset(new ThreadedDistanceCalculator(nthreads, bodies.size()));
        }
        if (nthreads == 1) {
            ac.reset( new PairWiseAccelerationCalculator(dc.get(), &bodies));
        }
        else {
            ac.reset( new ThreadedPairWiseAccelerationCalculator(dc.get(), &bodies, nAccThreads));
        }
        tsm.reset(new AccelerationTimeStepManager(amin, amax));
        if (pairWiseIntegrator) {
            integrator.reset(new RungeKutta(&system, ac.get(), tsm.get(), ie.get()));
        }
        else if (symplecticIntegrator) {
            integrator.reset(new SymplecticRknSB3AMclachlan(&system, ac.get(), tsm.get(), ie.get()));
        }
        else if (m4) {
            integrator.reset(new SymplecticRknSB3AM4Mclachlan(&system, ac.get(), tsm.get(), ie.get()));
        }
        else if (bs) {
            auto stepper = StepperFactory::createBurlirschStoer(absError, relError);
            integrator.reset(new ControlledIntegrator<BSStepperType>(
                &system, ac.get(), tsm.get(), ie.get(), stepper)
            );
        }
        else if (karp) {
            auto stepper = StepperFactory::createRKCashKarp(absError, relError);
            integrator.reset(new ControlledIntegrator<RKCashKarpStepperType>(
                &system, ac.get(), tsm.get(), ie.get(), stepper)
            );
        }
        else if (fehlberg) {
            auto stepper = StepperFactory::createRKFehlberg(absError, relError);
            integrator.reset(new ControlledIntegrator<RKFehlbergStepperType>(
                &system, ac.get(), tsm.get(), ie.get(), stepper)
            );
        }
        else {
            cerr << "No integrator specified" << endl;
            return 1;
        }
    unique_ptr<PositionPublisher> pp;
    if (plotSteps > 0) {
        if (fork() == 0) {
            //execl("plotter.py", "plotter.py", nullptr);
            execl("/usr/bin/mayavi2", "mayavi2", "plotter_mayavi.py", nullptr);
            cerr << "Error: could not launch plotter.py" << endl;
            return 1;
        }
        pp.reset(new PositionPublisher(system.getBodies()));
        pp->setNSteps(plotSteps);
        integrator->setPositionPublisher(pp.get());
    }
    if (bodyStr.empty()) {
        cout << "initial " << setprecision(10) << system << endl;
    }
    auto e0 = system.getEnergy();
    integrator->integrate();
    auto systemFinal = system;

    cout << "s.getTime " << system.getTime(SECOND) << " seconds" << endl;
    cout << "s.getTime " << system.getTime(DAY) << " days" << endl;
    cout << "rk time " << integrator->getElapsedTime(DAY) << " days "
        << integrator->getElapsedTime(SECOND) << " seconds" << endl;
    systemFinal.setTime(system.getTime(SECOND) + integrator->getElapsedTime(SECOND), SECOND);
    Vvector x(bodies.size());
    Vvector v(bodies.size());
    integrator->getXV(x, v);
    systemFinal.setXV(x, v);
    if (bodyStr.empty()) {
        cout << "final " << setprecision(10) << systemFinal << endl;
    }
    else {
        compareStateVectors(systemFinal);
    }
    cout << setprecision(6) << "elapsed time " << (integrator->getElapsedTime(DAY))
        << " days " << integrator->getElapsedTime(SECOND) << " seconds " <<  endl;
    cout << "nsteps " << integrator->nsteps() << endl;
    auto ef = systemFinal.getEnergy();
    cout << scientific << "frac energy change " << (ef/e0 - 1) << endl;
    timer.stop();
    auto udcTimer = UnthreadedDistanceCalculator::_timer;
    //auto tdcTimer = ThreadedDistanceCalculator::_timer;

    cout << "application timer: total elapsed " << timer.totalDuration()
                << " seconds, cycles " << timer.nCycles() << ", average per cycle: " << timer.meanDuration()
                << " seconds" << endl;
    /*
    cout << "unthreaded distance calculator timer: total elapsed " << udcTimer.totalDuration()
            << " seconds, cycles " << udcTimer.nCycles() << ", average per cycle: " << udcTimer.meanDuration()
            << " seconds" << endl;
    cout << "threaded distance calculator timers: total elapsed "
                << " seconds, cycles " << ", average per cycle: "
                << " seconds" << endl;
    for (auto& t: ThreadedDistanceCalculator::timers) {
        cout << t.totalDuration() << " " << t.nCycles() << " " << t.meanDuration() << endl;
    }
    */
    /*
    int i = 0;
    for (auto& tt: ThreadedDistanceCalculator::threadTimers) {
        cout << "thread " << i << endl;
        for (auto& t: tt) {
            cout << t.totalDuration() << " " << t.nCycles() << " " << t.meanDuration() << endl;
        }
        ++i;
    }
    */
    return 0;
    }
    catch (const KeplerException& x) {
        cerr << "Error: " << x.getMsg() << endl;
        return 1;
    }
}

