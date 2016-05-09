find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
set (datafiles
    acen.xml
)

foreach (file ${datafiles})
    configure_file (${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}/${file} COPYONLY)
endforeach (file)

# Link runTests with what we want to test and the GTest and pthread library

set(
	tests
	tXMLSystemFileReader
)

foreach (test ${tests})
	add_executable (${test}  ${test}.cc)
	target_link_libraries (${test} ${GTEST_LIBRARIES} pthread kepler xerces-c)
	add_test (${test} ${test})
endforeach (test)

