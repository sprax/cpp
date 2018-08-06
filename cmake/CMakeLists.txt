cmake_minimum_required(VERSION 3.5)
project(test_cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# list(APPEND CMAKE_PREFIX_PATH $ENV{DRAKE_BUILD_DIR})
# list(APPEND CMAKE_PREFIX_PATH $ENV{DRAKE_BUILD_DIR}/lib/cmake)

# IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#     find_package(Eigen3 REQUIRED)
# ENDIF()

# find_package(Boost 1.58 COMPONENTS serialization filesystem system program_options REQUIRED)
# include_directories(SYSTEM ${Boost_INCLUDE_DIR})

## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

#### PYTHON ENVIRONMENTS BEGIN ####
# list(APPEND AnacondaUsers "albertwu" "dave")
# list(APPEND PyenvUsers "sprax")
# if ($ENV{CIRCLECI})
#     find_package( Python2 2.7.12 REQUIRED Interpreter Development)
#     set(PYTHON_LIBRARIES ${Python2_LIBRARIES} )
#     set(PYTHON_INCLUDE_DIRS "${Python2_INCLUDE_DIRS}"
#         "/usr/lib/python2.7/dist-packages/numpy/core/include")
# elseif ($ENV{USER} IN_LIST PyenvUsers)
#     set(PYTHON_LIBRARIES $ENV{HOME}/.pyenv/versions/2.7.15/lib/libpython2.7.a)
#     set(PYTHON_INCLUDE_DIRS $ENV{HOME}/.pyenv/versions/2.7.15/include/python2.7
#         $ENV{HOME}/.pyenv/versions/2.7.15/lib/python2.7/site-packages/numpy/core/include)
# elseif ($ENV{USER} IN_LIST AnacondaUsers)
#     set(PYTHON_BASE_DIR "$ENV{HOME}/anaconda2/envs/py27")
#     set(PYTHON_LIBRARIES "${PYTHON_BASE_DIR}/lib/libpython2.7.dylib")
#     set(PYTHON_INCLUDE_DIRS "${PYTHON_BASE_DIR}/include/python2.7"
#         "${PYTHON_BASE_DIR}/lib/python2.7/site-packages/numpy/core/include" )
# else()
#     message("####################################################################################################################")
#     message("# hey $ENV{USER} with home directory $ENV{HOME}:")
#     message("# If the compiler says fatal error: 'Python.h' file not found', see the PYTHON ENVIRONMENT section in CMakeLists.txt")
#     message("####################################################################################################################")
# endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(SharedLibExt "dylib")
else()
        set(SharedLibExt "so")
endif()
message("============== That SharedLibExt is ${SharedLibExt} ==============")

find_package(PythonLibs 2.7.12 COMPONENTS Interpreter Development)
if (${PYTHONLIBS_FOUND})
    message("Found PythonLibs: PYTHON_LIBRARIES: [${PYTHON_LIBRARIES}]")
    message("Found PythonLibs: PYTHON_INCLUDE_DIRS: [${PYTHON_INCLUDE_DIRS}]")
else()
endif()
return()

if (True)
    execute_process(COMMAND python2-config --libs OUTPUT_VARIABLE PYLIB)
    string(REGEX REPLACE "\n$" "" PYLIB "${PYLIB}")
    message("What I got of PYLIB is sick: ${PYLIB} <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<")
endif()

if (False)
  if ($ENV{Python2_ROOT_DIR})
    set(Python2_ROOT_DIR $ENV{Python2_ROOT_DIR})
    message("=========================== Python2_ROOT_DIR is ${Python2_ROOT_DIR}")
  else()
    message("--------------------------- Python2_ROOT_DIR is not defined . . .")
    IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(Python2_ROOT_DIR "/Users/sprax/.pyenv/versions/2.7.15")
    ELSE()
        set(Python2_ROOT_DIR "/usr/lib/python2.7")
    ENDIF()

    message("--------------------------- Python2_ROOT_DIR is now ${Python2_ROOT_DIR}")
  endif()
  return()
endif()


execute_process(COMMAND python-config --prefix OUTPUT_VARIABLE PYTHON_SEARCH_PATH)
string(REGEX REPLACE "\n$" "" PYTHON_SEARCH_PATH "${PYTHON_SEARCH_PATH}")
file(GLOB_RECURSE PYTHON_DY_LIBS ${PYTHON_SEARCH_PATH}/lib/libpython2.7*.${SharedLibExt})
if (PYTHON_DY_LIBS)
    list(GET PYTHON_DY_LIBS 0 PYTHON_LIBRARY)
    message(">>>> Find shared libpython: ${PYTHON_LIBRARY}")
else()
    message(WARNING ">>>> Cannot find shared libpython, try find_package")
endif()

if (False) 
    message("True True True True True True True True True True")
endif()
return()

if (${NO_WAY})
  if ($ENV{CIRCLECI})
    find_package(Python2 2.7.12 REQUIRED Interpreter Development)
    set(PYTHON_LIBRARIES ${Python2_LIBRARIES} ) #"${PYTHON_BASE_DIR}/lib/libpython2.7.dylib")
    set(PYTHON_INCLUDE_DIRS "${Python2_INCLUDE_DIRS}"
        "/usr/lib/python2.7/dist-packages/numpy/core/include")
  else()
    find_package(Python2 2.7.12)
    # find_package(PythonLibs 2.7 REQUIRED) # deprecated: https://cmake.org/cmake/help/latest/module/FindPythonInterp.html
    if (${Python2_LIBRARIES})
        find_package( Python2 2.7.12 REQUIRED COMPONENTS Interpreter Development)
        set(PYTHON_LIBRARIES ${Python2_LIBRARIES} )
        set(PYTHON_INCLUDE_DIRS "${Python2_INCLUDE_DIRS}"
            "${Python2_LIBRARY_DIRS}/python2.7/dist-packages/numpy/core/include"
            "${Python2_LIBRARY_DIRS}/python2.7/site-packages/numpy/core/include")
    else()
        set(PYTHON_LIBRARIES $ENV{HOME}/.pyenv/versions/2.7.15/lib/libpython2.7.a)
        set(PYTHON_INCLUDE_DIRS $ENV{HOME}/.pyenv/versions/2.7.15/include/python2.7
            $ENV{HOME}/.pyenv/versions/2.7.15/lib/python2.7/site-packages/numpy/core/include)
    endif()
  endif()
endif()


message(STATUS "CMake found this configuration for Python2:")
message(STATUS "Python2_LIBRARIES:    [${Python2_LIBRARIES}]")
message(STATUS "Python2_INCLUDE_DIRS: [${Python2_INCLUDE_DIRS}]")
message(STATUS "Python2_LIBRARY_DIRS: [${Python2_LIBRARY_DIRS}]")
message(STATUS "------------------------------------------------------------------")
message(STATUS "Building with these values for the Python2 libraries and includes:")
message(STATUS "PYTHON_LIBRARIES [${PYTHON_LIBRARIES}]")
message(STATUS "PYTHON_INCLUDE_DIRS [${PYTHON_INCLUDE_DIRS}]")
return()

#### PYTHON ENVIRONMENTS END ####

find_package(Protobuf REQUIRED)
# find_package(Protobuf 2.6.1 EXACT REQUIRED)
# find_package(Protobuf 3.6.0 EXACT REQUIRED)
if (NOT ${PROTOBUF_LIBRARY})
    set(PROTOBUF_LIBRARY protobuf)
endif()

include_directories(
    include
    $ENV{DRACULA_INCLUDE_DIR}
    $ENV{JSON_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)
