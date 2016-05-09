find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library

set(
	tests
	tAccelerationTimeStepManager
)

foreach (test ${tests})
	add_executable (${test}  ${test}.cc)
	target_link_libraries (${test} ${GTEST_LIBRARIES} pthread kepler xerces-c)
	add_test (${test} ${test})
endforeach (test)

