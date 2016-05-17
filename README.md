# kepler++ (keplerpp project on github)
Pair-wise N-body code written in C++

## GETTING

This project is hosted on github. To retrieve the source, it is recommended that you create
a directory called kepler++ or keplerpp and cd into it, and then run

```
git clone https://github.com/davemehringer/keplerpp src
```

which will clone the source into a directory called `src`.

## DEPENDENCES

Google Test https://github.com/google/googletest

Boost odeint (at least version 1.59) http://headmyshoulder.github.io/odeint-v2

## BUILDING

CMake is used for configuration. It is recommended you create a build directory
at the same level as the source directory (which we call src here, and cd into the build directory. 
Then do, at a minimum,
```
cmake ../src
```

If you do not have the google test package installed in a standard location, you will need to specify
where the associated header files and library are on the cmake command line, eg

```
cmake ../src -DGTEST_INCLUDE_DIR=/path/to/gtest/googletest-release-1.7.0/include -DGTEST_MAIN_LIBRARY=/path/to/gtest/libgtest.a
``` 

If you do not have version 1.59 or later of boost installed in a standard location, you will need
to specify the boost root directory on the cmake command line, eg
```
cmake ../src -DBOOST_ROOT=$HOME/boost_1_59_0

## COMMAND LINE OPTIONS

The main application is kep, built from kepler.cc. The command line options are:
```
-a amin,amax
    This is used for some, but not all integrators. Both amin and amax
    are required if -a is specified. They provide the conditions under
    which the time step is changed. For each step, the absolute value
    of the relative change of each of the three components of the
    acceleration vector for each body are computed:
       
        delta_a_i = abs(a_i_previous_step - a_i_current_step)/magnitude_a_current_step

    where 

        magnitude_a_current_step = sqrt(
            a_0_current_step*a_0_current_step
            + a_1_current_step*a_1_current_step
            + a_2_current_step*a_2_current_step
        )

    is the magnitude of the acceleration vector for the current step.
    If *all* values of delta_a_i for *all* bodies are less than amin, the
    time step is increased for the next step. If *any* value of delta_a_i
    for *any* body, the positions and velocities are reset to the values
    they had at the end of the previous step, and the current step is
    repeated, using a smaller value of the time step. For solar system
    computations, If they are not specified, the default values are
    amin=0.002 and amax=0.005.

-b body1,body2,...
    For solar system computations, this is the list of bodies
    that should be included in the computations. In this case, the code
    also requires the start Julian Day to be specified using the -t option.
    The posiitons and velocities of the specified bodies will be
    retrieved from the JPL Horizons telnet server to initialize the
    system. Recognized solar system bodies can be found in
    SolarSystem/SSObjects.cc. Only one of the -b or -f options may
    be specified. If neither is specified, a simple two body system
    is created.

-c absErr,relErr 
    Similar to the -a option, these are values which determine the
    allowable error for integraton methods which are not governed by
    the -a settings. Default values are absErr=1e-10 and relErr=1e-10.

-e value
    This indicates that the simulation should be halted after value simulated
    days have elapsed. Another way to specify when to end the simulation
    is via the -s parameter. Only one of -e or -s may be specified. If neither
    -e nor -s is specified, the default behavior is to use -s 200000. It
    is highly recommended to explicitly specify one of these switches.

-f xmlFile
    XML file containing the description of the initial state of the system.
    The -b and -f options are mutually exclusive. If neither is specified,
    a simple two body system is created.

-h nthreads
    The number of threads to use for the computation of distances. For a system
    n bodies it is recommended that nthreads be less than n*(n-1)/2 and that 
    nthreads also be less than or equal to the number of cores on the processing
    machine. In theory, the optimal number of threads should be equal to 
    `min(number of cores, n*(n-1)/2)`. Default is 1.
 

-i integrator
    The integrator to use. Must be specified. Supported values are:
        p - use basic fourth order Runge-Kutta method, use -a to control time step size
        s - use symplectic MacLachlan stepper for separable Hamiltonian system, as
            implemented in boost. Use -a for time step size control.
        m4 - use symplectic MacLachlan M4 stepper for separable Hamiltonian system, as
            implemented in boost. Use -a for time step size control.
        bs - use Burlirsch-Stoer stepper as implemented in boost. Use -c for step
            size control.
        karp - use Runge Kutta Cash Karp 5(4) stepper, as implemented in boost.
            Use -c to control time step size.
        fehl - use the Runge-Kutta-Fehlberg 78 stepper, as implemented in boost.
            Use -c to contorl time step size.

-j nthreads
    The number of threads to use for the computation of accelerations. For a system
    n bodies it is recommended that nthreads be less than n*(n-1)/2 and that 
    nthreads also be less than or equal to the number of cores on the processing
    machine. In theory, the optimal number of threads should be equal to 
    `min(number of cores, n*(n-1)/2)`. Default is 1.
 
-p nsteps
    Update the plot every nsteps. If not specified, no plotting is used. A small value
    will noticeably impact performance. A value of around 50 is normally a good choice.

-s nsteps
    As an alternative to the -e option, this option indicates that the simulation should
    be ended after nsteps. Exactly one of -e or -s must be specified.

-t start_time
    Start time, in days, of the simulation. Normally only useful for solar system
    simulations (-b), in which case this value is in Julian Days and must be specified.
    
```

## EXAMPLES

Run a solar system simulation for the bodies given in the -b option, starting at Julian Day 2457400
(given by the -t option), using four acceleration calculation threads (-j), four distance calculation
threads (-h), using the Burlirsch-Stoer stepper algorithm (-i), with maximum absolute and 
relative errors of 1e-8 and 1e-8 (-c), ending 10,000 simulated days after the start (-e),
and update the plot every 50 steps.

```
./kep -t 2457400 -b "mercury,venus,earth,moon,mars,phobos,deimos,jupiter,amalthea,io,europa,ganymede,callisto,himalia,elara, ananke, carme, leda, pasiphae, sinope,metis, themisto, callirrhoe, megaclite,taygete, chaldene, harpalyke, kalyke, iocaste, erinome, isonoe, praxidike, autonoe, thyone, saturn,janus,epimetheus, mimas,enceladus,tethys,dione,rhea,titan,iapetus,hyperion,phoebe, prometheus,pandora, uranus,miranda,ariel,umbriel,titania,oberon,neptune,triton,nereid,pluto,charon,nix,hydra,kerberos" -j 4 -h 4 -i bs -c 1e-08, 1-e08  -e 10000 -p 50
```

## THIRD PARTY CODE INCLUDED IN THIS PACKAGE

apps/osc_tbl and apps/state_tbl are from ftp://ssd.jpl.nasa.gov/pub/ssd
and are used to automate retrieval of JPL Horizons data.
