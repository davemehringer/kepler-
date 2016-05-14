# kepler++ (keplerpp project on github)
Pair-wise N-body code written in C++

COMMAND LINE OPTIONS

The main application is kep, built from kepler.cc. The command line options are:

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
    machine.

-i integrator
    The integrator to use. Supported values are:
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
    

THIRD PARTY CODE INCLUDED IN THIS PACKAGE

apps/osc_tbl and apps/state_tbl are from ftp://ssd.jpl.nasa.gov/pub/ssd
and are used to automate retrieval of JPL Horizons data.
