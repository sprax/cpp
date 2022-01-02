cmake_minimum_required(VERSION 3.12)
project(test_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PYBIND11_CPP_STANDARD -std=c++17)

set(CMAKE_CXX_FLAGS "-D_GLIBCXX_USE_CXX11_ABI=1 -pthread -fPIC")

# WARNINGS: Set -Wall and -Wextra, but silence some noisy ones such as -Wshadow
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wsign-compare -Wuninitialized -Wunused-variable")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-unused-parameter")

# Different compiler flags for Mac and Linux:
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register -Wunused-const-variable")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized")
    set(STDCXX_FS "stdc++fs")
endif()
IF (($ENV{CIRCLECI} MATCHES "false") AND ($ENV{USER} MATCHES "sprax"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
ENDIF()
message("CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
message("CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})

IF (($ENV{CIRCLECI} MATCHES "true") OR ($ENV{USER} MATCHES "sprax"))
    # Set global property (all targets are impacted)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
ENDIF()

# configure_file(data/test_data.txt "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)
# @file(COPY data/aux DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

# list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

find_package(GTest REQUIRED)

## load in pkg-config support
# find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
# pkg_check_modules(PC_ZeroMQ QUIET zmq)

# find_package(Boost 1.65.1 REQUIRED COMPONENTS date_time regex program_options filesystem system thread)

find_package( Python3 3.9 REQUIRED Interpreter Development)
set(PYTHON_LIBRARIES ${Python3_LIBRARIES} )

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(PYTHON_LIBRARIES ${Python3_LIBRARIES} )
else()
    set(PYTHON_LIBRARIES "/usr/lib/x86_64-linux-gnu/libpython3.6m.so")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(PYTHON_INCLUDE_DIRS
        "${Python3_INCLUDE_DIRS}"    # kitchen sink
        "${Python3_LIBRARY_DIRS}/python3.6/dist-packages/numpy/core/include"
        "${Python3_LIBRARY_DIRS}/python3.6/site-packages/numpy/core/include"
    )
else()
    set(PYTHON_INCLUDE_DIRS
        "/usr/include/python3.6"
        "${Python3_INCLUDE_DIRS}"    # kitchen sink
        "/usr/local/lib/python3.6/dist-packages/numpy/core/include"
        "/usr/local/lib/python3.6/site-packages/numpy/core/include/"
    )
endif()

message(STATUS "Python3_LIBRARIES: ${Python3_LIBRARIES}")
message(STATUS "Python3_INCLUDE_DIRS: ${Python3_INCLUDE_DIRS}")
message(STATUS "Python3_LIBRARY_DIRS: ${Python3_LIBRARY_DIRS}")
message(STATUS "PYTHON_INCLUDE_DIRS: ${PYTHON_INCLUDE_DIRS}")

#### PYTHON ENVIRONMENTS END ####

include_directories( BEFORE
    include
    src
    # utils
    /usr/local/include
)

#
# add_library(test_util SHARED
#     src/spiral_sampler.cc
# )
#
# # Set property for one target only
# set_property(TARGET test PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")
#
# target_link_libraries(test
#     ${PYTHON_LIBRARIES}
# )
#
#
# set_property(TARGET test_utils PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")
#
#

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# # # # # BEG: PYBIND11 # # # # #
# # NOTE: explicitly adding pybind11 may cause: fatal error: 'numpy/arrayobject.h' file not found
find_package(pybind11 REQUIRED) # this would break the Python2 package

# add_subdirectory(externals/pybind11)

#-------------------
# Unit Testing
#-------------------
enable_testing()

# Glob the test files instead of setting a list.
# Rerun cmake after adding or removing files.
# To filter or count only the generated list, try `cd build; cmake .. 2>&1 | grep test_ | wc`
file(GLOB_RECURSE GTEST_SRC_FILES "${CMAKE_CURRENT_LIST_DIR}/test??/test_*/test_*.cpp")
foreach(_file ${GTEST_SRC_FILES})
    message(STATUS ${_file})
endforeach()

# Tests that do not need to link against libtest_utils:
# Place your newest tests first in this list, unless they are slow to build or run.

# slowest tests last, please.

foreach(_file ${GTEST_SRC_FILES})
    get_filename_component(_name ${_file} NAME_WE)
    add_executable(${_name} ${_file})
    target_link_libraries(${_name}
        ${CMAKE_THREAD_LIBS_INIT}
        ${STDCXX_FS}
        gtest gtest_main
    )
    add_test(${_name} ${_name})
    set_tests_properties(${_name} PROPERTIES TIMEOUT 90)
endforeach()
