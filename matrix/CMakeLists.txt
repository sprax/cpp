cmake_minimum_required(VERSION 3.13)
project(matrix)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PYBIND11_CPP_STANDARD -std=c++14)

set(CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=1 -pthread")

# WARNINGS: Set -Wall and -Wextra, but silence some noisy ones such as -Wshadow
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wsign-compare -Wuninitialized")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-unused-parameter")

# Different compiler flags for Mac and Linux:
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register -Wunused-const-variable")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized")
endif()
message("CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
message("CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})

IF (($ENV{CIRCLECI} MATCHES "true") OR ($ENV{USER} MATCHES "sprax"))
	# Set global property (all targets are impacted)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
ENDIF()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	find_package(Eigen3 3.3.5 QUIET CONFIG)
ENDIF()


list(APPEND CMAKE_PREFIX_PATH $ENV{DRAKE_BUILD_DIR})
list(APPEND CMAKE_PREFIX_PATH "$ENV{DRAKE_BUILD_DIR}/lib/cmake")
list(APPEND CMAKE_PREFIX_PATH $ENV{TORCH_CMAKE_DIR})

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

find_package(GTest REQUIRED)

find_package( Python3 3.6 REQUIRED Interpreter Development)
set(PYTHON_LIBRARIES ${Python3_LIBRARIES} )
## set(PYTHON_INCLUDE_DIRS "${Python2_INCLUDE_DIRS}"
    ## "${Python2_LIBRARY_DIRS}/python2.7/dist-packages/numpy/core/include"
    ## "${Python2_LIBRARY_DIRS}/python2.7/site-packages/numpy/core/include")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(PYTHON_LIBRARIES ${Python3_LIBRARIES} )
else()
    set(PYTHON_LIBRARIES "/usr/lib/x86_64-linux-gnu/libpython3.6m.so") #$ENV{LOCAL_PYTHON_PATH}/lib/libpython2.7.so )
endif()

set(Python3_ROOT_DIR $ENV{LOCAL_PYTHON_PATH})
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(PYTHON_INCLUDE_DIRS
		"${Python3_INCLUDE_DIRS}"	# kitchen sink
		"${Python3_LIBRARY_DIRS}/python3.6/dist-packages/numpy/core/include"
		"${Python3_LIBRARY_DIRS}/python3.6/site-packages/numpy/core/include"
	)
else()
	set(PYTHON_INCLUDE_DIRS
		"/usr/include/python3.6" #$ENV{LOCAL_PYTHON_PATH}/include/python3.6
		"${Python3_INCLUDE_DIRS}"	# kitchen sink
		"/usr/local/lib/python3.6/dist-packages/numpy/core/include"
		"/usr/local/lib/python3.6/site-packages/numpy/core/include/"
	)
endif()

message(STATUS "Python3_LIBRARIES: ${Python3_LIBRARIES}")
message(STATUS "Python3_INCLUDE_DIRS: ${Python3_INCLUDE_DIRS}")
message(STATUS "Python3_LIBRARY_DIRS: ${Python3_LIBRARY_DIRS}")
message(STATUS "PYTHON_INCLUDE_DIRS: ${PYTHON_INCLUDE_DIRS}")
#### PYTHON ENVIRONMENTS END ####

include_directories(
	include
)

add_library(matrix SHARED
	src/matrix.cc
)

# Set property for one target only
set_property(TARGET matrix PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")

target_link_libraries(matrix
	${PYTHON_LIBRARIES}
)

add_executable(matrices src/matrices.cc)
target_link_libraries(matrices
	matrix
)
set_property(TARGET matrices PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

#-------------------
# Unit Testing
#-------------------
enable_testing()

#   newest tests first, please.
set(GTEST_SRC_FILES
	tests/test_matrix.cc
)

foreach(_file ${GTEST_SRC_FILES})
	get_filename_component(_name ${_file} NAME_WE)
	add_executable(${_name} ${_file})
	target_link_libraries(${_name} matrix
		gtest gtest_main
		${CMAKE_THREAD_LIBS_INIT}
	)
	add_test(${_name} ${_name})
	set_tests_properties(${_name} PROPERTIES TIMEOUT 30)
endforeach()
