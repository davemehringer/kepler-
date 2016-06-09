cmake_minimum_required (VERSION 2.6)
project (kepler)
# The version number.
set (Kepler_VERSION_MAJOR 0)
set (Kepler_VERSION_MINOR 1)
 
# configure a header file to pass some of the CMake settings
# to the source codecmake copy header file to include directory
configure_file (
  "${PROJECT_SOURCE_DIR}/config/KeplerConfig.h.in"
  "${PROJECT_BINARY_DIR}/config/KeplerConfig.h"
)

find_package(
    Boost 1.59.0 REQUIRED 
)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    #target_link_libraries(${Boost_LIBRARIES})
endif()

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

SET( CMAKE_CXX_FLAGS "-std=c++11 -pthread -Wall" ) 

include_directories (${CMAKE_SOURCE_DIR})
 
add_library(
	kepler SHARED 
    AccelerationCalculator/PairWiseAccelerationCalculator.cc
    AccelerationCalculator/ThreadedPairWiseAccelerationCalculator.cc
	BodyFrameRotMatrix/EarthFrameRotMatrix.cc
	BodyFrameRotMatrix/JupiterFrameRotMatrix.cc
	BodyFrameRotMatrix/MarsFrameRotMatrix.cc
	BodyFrameRotMatrix/NeptuneFrameRotMatrix.cc
	BodyFrameRotMatrix/SaturnFrameRotMatrix.cc
	BodyFrameRotMatrix/UranusFrameRotMatrix.cc
	DistanceCalculator/ThreadedDistanceCalculator.cc
	DistanceCalculator/UnthreadedDistanceCalculator.cc
    Exception/KeplerException.cc
	IntegrationEnder/IntegrationEnder.cc
    IntegrationEnder/ElapsedTimeIntegrationEnder.cc
    IntegrationEnder/NStepsIntegrationEnder.cc
    # Integrator/BulirschStoer.cc
    Integrator/ControlledIntegratorBase.cc
	Integrator/Integrator.cc
	Integrator/PositionPublisher.cc
	Integrator/RungeKutta.cc
	Integrator/StepperFactory.cc
	Integrator/Symplectic.cc
	Integrator/SymplecticRknSB3AM4Mclachlan.cc
    Integrator/SymplecticRknSB3AMclachlan.cc
	IO/IO.cc
    IO/XMLSystemFileReader.cc
	SolarSystem/JPLHorizonsRetriever.cc
	Math/Math.cc
	Math/RotMatrix3x3.cc
	Math/Vector.cc
	Math/VectorTester.cc
    OS/Timer.cc
	Physics/Body.cc
	Physics/NBodySystem.cc
	Physics/StateVectorsOrbitalElements.cc
	SolarSystem/SSComparer.cc
    SolarSystem/SSObjects.cc
    TimeStepManager/AccelerationTimeStepManager.cc
)

include (CTest)

add_subdirectory (AccelerationCalculator/test)
add_subdirectory (DistanceCalculator/test)
add_subdirectory (Integrator/test)
add_subdirectory (IO/test)
add_subdirectory (Math/test)
add_subdirectory (Physics/test)
add_subdirectory (SolarSystem/test)
add_subdirectory (TimeStepManager/test)

add_executable(kep apps/kepler.cc)

#add_executable(xmlparse apps/xmlparse.cc)

target_link_libraries ( kep pthread kepler xerces-c )

#target_link_libraries ( xmlparse xerces-c kepler )

enable_testing()

INSTALL(TARGETS kepler DESTINATION "lib")
#INSTALL(TARGETS xmlparse DESTINATION "lib")

INSTALL(
	FILES
	AccelerationCalculator/AccelerationCalculator.h
	AccelerationCalculator/PairWiseAccelerationCalculator.h
	AccelerationCalculator/ThreadedPairWiseAccelerationCalculator.h
	BodyFrameRotMatrix/BodyFrameRotMatrix.h
	BodyFrameRotMatrix/EarthFrameRotMatrix.h
    BodyFrameRotMatrix/JupiterFrameRotMatrix.h
    BodyFrameRotMatrix/MarsFrameRotMatrix.h
    BodyFrameRotMatrix/NeptuneFrameRotMatrix.h
    BodyFrameRotMatrix/SaturnFrameRotMatrix.h
    BodyFrameRotMatrix/UranusFrameRotMatrix.h
    DistanceCalculator/DistanceCalculator.h
    DistanceCalculator/ThreadedDistanceCalculator.h
    DistanceCalculator/UnthreadedDistanceCalculator.h
    Exception/KeplerException.h
	IntegrationEnder/IntegrationEnder.h
	IntegrationEnder/ElapsedTimeIntegrationEnder.h
	IntegrationEnder/NStepsIntegrationEnder.h
	# Integrator/BulirschStoer.h
    Integrator/ControlledIntegrator.h
    Integrator/ControlledIntegrator.tcc
    Integrator/ControlledIntegratorBase.h
	Integrator/Integrator.h
	Integrator/RungeKutta.h
    Integrator/StepperFactory.h
	Integrator/Symplectic.h
    Integrator/SymplecticRknSB3AM4Mclachlan.h
	Integrator/SymplecticRknSB3AMclachlan.h
	IO/IO.h
	IO/XMLSystemFileReader.h
	SolarSystem/JPLHorizonsRetriever.h
    Math/Constants.h
	Math/Math.h
	Math/RotMatrix3x3.h
	Math/Types.h
    Math/Vector.h
    Math/VectorTester.h
	Physics/Body.cc
    Physics/NBodySystem.cc
    Physics/StateVectorsOrbitalElements.cc
	Integrator/PositionPublisher.h
	OS/Timer.h
	SolarSystem/SSComparer.h
	SolarSystem/SSObjects.h
	TimeStepModifier/AccelerationTimeStepMananger.h
	TimeStepModifier/TimeStepManager.h
	DESTINATION "include/kepler"
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plotter.py
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/apps/plotter.py
    ${CMAKE_CURRENT_BINARY_DIR}/plotter.py
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/apps/plotter.py
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plotter_mayavi.py
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/apps/plotter_mayavi.py
    ${CMAKE_CURRENT_BINARY_DIR}/plotter_mayavi.py
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/apps/plotter_mayavi.py
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/osc_tbl
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/apps/osc_tbl
    ${CMAKE_CURRENT_BINARY_DIR}/osc_tbl
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/apps/osc_tbl
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/state_tbl
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/apps/state_tbl
    ${CMAKE_CURRENT_BINARY_DIR}/state_tbl
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/apps/state_tbl
)

add_custom_target(plotter ALL DEPENDS plotter.py) 
add_custom_target(plotter_mayavi ALL DEPENDS plotter_mayavi.py) 
add_custom_target(osctbl ALL DEPENDS osc_tbl) 
add_custom_target(statetbl ALL DEPENDS state_tbl) 

#set(CONFIG_PY_DIR ${PROJECT_SOURCE_DIR})
#configure_file(plotting.py.in ${PROJECT_BINARY_DIR}/plotting.py @ONLY)
#set(CONFIG_PY_DIR ${PYTHON_INSTALL_DIR})
#configure_file(program.py.in
#  ${PROJECT_BINARY_DIR}/installFiles/program.py @ONLY)
#install(PROGRAMS ${PROJECT_BINARY_DIR}/installFiles/plotter.py
#  DESTINATION bin)

#add_executable(bs_example boost/examples/bulirsch_stoer.cpp)

#add_executable(solar_system_boost boost/examples/solar_system.cpp)
