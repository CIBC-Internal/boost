#  For more information, please see: http://software.sci.utah.edu
# 
#  The MIT License
# 
#  Copyright (c) 2015 Scientific Computing and Imaging Institute,
#  University of Utah.
# 
#  
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included
#  in all copies or substantial portions of the Software. 
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.

# borrowed cmake directives from https://github.com/maidsafe/MaidSafe/blob/master/cmake_modules/add_boost.cmake

cmake_minimum_required(VERSION 2.8)
project(Boost)

if(APPLE)
  set(BOOST_TOOLSET "darwin")
elseif(WIN32)
  set(BOOST_TOOLSET "msvc")
elseif(UNIX)
  # CMAKE_CXX_COMPILER_ID Clang definitely available in cmake 2.8.10 and newer
  if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(BOOST_TOOLSET "clang")
    set(BOOST_UNIX_CLANG ON)
  else()
    set(BOOST_TOOLSET "gcc")
  endif()
else()
  message(ERROR "Unsupported platform. Cannot detect Boost toolset.")
endif()

option(BUILD_PYTHON "Build Python interface library" OFF)
option(FORCE_64BIT_BUILD "Force build to be 64 bit" OFF)
option(FORCE_QUIET_BUILD "Suppress all informational messages" ON)
option(SHARED_BUILD "Build shared libraries (static is default)." OFF)
# TODO: set in superbuild
option(STATIC_RUNTIME_LINK "Link to static C++ runtime." OFF)
set(N_PARALLEL_BUILD_COMMANDS "4" CACHE STRING "Run up to N build commands in parallel")

if(BUILD_PYTHON)
  if(Python_DIR)
    find_package(Python CONFIGS PythonConfig.cmake HINTS ${Python_DIR} NO_SYSTEM_ENVIRONMENT_PATH)
    if(NOT Python_FOUND)
      message(FATAL_ERROR "Python library not found in ${Python_DIR}")
    endif()
    include(${SCI_PYTHON_USE_FILE})
  else()
    message(FATAL_ERROR "Python library is required to compile Boost when BUILD_PYTHON is on.")
  endif()
endif()

if(WIN32)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}" WIN32_BOOST_BUILD_PREFIX)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/lib" WIN32_BOOST_BUILD_LIB_DIR)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" WIN32_BOOST_BUILD_INCLUDE_DIR)
  set(BOOST_BUILD_PREFIX ${WIN32_BOOST_BUILD_PREFIX})
  set(BOOST_BUILD_LIB_DIR ${WIN32_BOOST_BUILD_LIB_DIR})
  set(BOOST_BUILD_INCLUDE_DIR ${WIN32_BOOST_BUILD_INCLUDE_DIR})
else()
  set(BOOST_BUILD_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
  set(BOOST_BUILD_LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib")
  set(BOOST_BUILD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# set libary list empty in project config file and set libaries on command line
set(BOOST_LIBRARIES)

if(WIN32)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bootstrap.bat" WIN32_BOOTSTRAP_COMMAND)
  set(BOOTSTRAP_COMMAND "${WIN32_BOOTSTRAP_COMMAND}")
else()
  # assuming *nix platforms that support bash scripts
  #set(BOOTSTRAP_COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/bootstrap.sh")
  set(BOOTSTRAP_COMMAND "./bootstrap.sh")
endif()

# not sure if this has any effect on Windows, but not rejected either
set(BOOTSTRAP_ARGS "--prefix=${CMAKE_CURRENT_BINARY_DIR}")
if(BOOST_UNIX_CLANG)
  list(APPEND BOOTSTRAP_ARGS "--with-toolset=clang")
endif()

if(SCI_BOOST_LIBRARIES)
  set(BOOTSTRAP_LIB_LIST)
  # trailing comma is OK
  foreach(lib ${SCI_BOOST_LIBRARIES})
    set(BOOTSTRAP_LIB_LIST "${BOOTSTRAP_LIB_LIST}${lib},")
  endforeach()
  if(BUILD_PYTHON)
    set(BOOTSTRAP_LIB_LIST "${BOOTSTRAP_LIB_LIST}python")
  endif()
  list(APPEND BOOTSTRAP_ARGS "--with-libraries=${BOOTSTRAP_LIB_LIST}")
endif()

if (BUILD_PYTHON)
  if(WIN32)
    # TO_NATIVE_PATH does not escape backslashes
    file(TO_NATIVE_PATH "${SCI_PYTHON_EXE}" TMP)
    string(REPLACE "\\" "\\\\" WIN32_PYTHON_EXE ${TMP})
    file(TO_NATIVE_PATH "${SCI_PYTHON_INCLUDE}" TMP)
    string(REPLACE "\\" "\\\\" WIN32_PYTHON_INCLUDE ${TMP})
    file(TO_NATIVE_PATH "${SCI_PYTHON_LIBRARY_DIR}" TMP)
    string(REPLACE "\\" "\\\\" WIN32_PYTHON_LIBRARY_DIR ${TMP})
	configure_file(user-config.jam.in user-config.jam @ONLY)
    list(APPEND BOOTSTRAP_ARGS
	  "--user-config=user-config.jam"
    )
  else()
    # TODO: try user-config.jam instead...
    list(APPEND BOOTSTRAP_ARGS
      "--with-python=${SCI_PYTHON_EXE}"
      "--with-python-root=${SCI_PYTHON_ROOT}"
      "--with-python-version=${SCI_PYTHON_VERSION_SHORT}"
    )
  endif()
endif()


add_custom_target(bootstrap ALL
  COMMAND ${BOOTSTRAP_COMMAND} ${BOOTSTRAP_ARGS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})  


if(WIN32)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/b2${CMAKE_EXECUTABLE_SUFFIX}" WIN32_B2_COMMAND)
  set(B2_COMMAND ${WIN32_B2_COMMAND})
else()
  set(B2_COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/b2")
endif()

if (CMAKE_GENERATOR MATCHES "Makefiles" AND CMAKE_BUILD_TYPE MATCHES "Debug")
  set(VARIANT "debug")
elseif(CMAKE_GENERATOR MATCHES "Makefiles")
  # build type may not necessarily be set and release is a safe default
  set(VARIANT "release")
else()
  SET(VARIANT "release,debug")
endif()

set(B2_ARGS "variant=${VARIANT}" "threading=multi")

if(SHARED_BUILD)
  list(APPEND B2_ARGS "link=shared")
else()
  list(APPEND B2_ARGS "link=static")
endif()

if(STATIC_RUNTIME_LINK)
  list(APPEND B2_ARGS "runtime-link=static")
else()
  list(APPEND B2_ARGS "runtime-link=shared")
endif()

if(FORCE_64BIT_BUILD)
  list(APPEND B2_ARGS "address-model=64")
endif()

if(APPLE)
  # For use of -Wno-c99-extensions flag, see Boost ticket #10276 
  list(APPEND B2_ARGS "toolset=clang" "cxxflags=-std=c++11" "cxxflags=-stdlib=libc++" "cxxflags=-Wno-c99-extensions" "linkflags=-stdlib=libc++")
elseif(WIN32)
  set(TOOLSET "msvc")
  if(MSVC10)
    set(TOOLSET "msvc-10.0")
  elseif(MSVC11)
    set(TOOLSET "msvc-11.0")
  elseif(MSVC12)
    set(TOOLSET "msvc-12.0")
  endif()

  message(STATUS "Configuring Windows build with toolset ${TOOLSET}")
  # list libraries and exit: --show-libraries
  list(APPEND B2_ARGS
        "toolset=${TOOLSET}"
        "architecture=x86"
        "--disable-icu")
elseif(UNIX)
  if(BOOST_UNIX_CLANG)
    # For use of -Wno-c99-extensions flag, see Boost ticket #10276 
    list(APPEND B2_ARGS "toolset=clang" "cxxflags=-Wno-c99-extensions")
  endif()
  list(APPEND B2_ARGS "cxxflags=-fPIC")
endif()

foreach(flag ${SCI_BOOST_CXX_FLAGS})
  list(APPEND B2_ARGS "cxxflags=${flag}")
endforeach()

# run b2 with build-dir, stagedir
# not currently supporting zlib, bzip2

if(FORCE_QUIET_BUILD)
  list(APPEND B2_ARGS "-d0")
endif()

list(APPEND B2_ARGS
  "-j${N_PARALLEL_BUILD_COMMANDS}"
  "-q"
  "-sNO_ZLIB=1"
  "-sNO_BZIP2=1"
  "--build-type=complete"
  "--build-dir=${CMAKE_CURRENT_BINARY_DIR}"
  "--stagedir=${CMAKE_CURRENT_BINARY_DIR}"
  "--layout=tagged"
  "stage")

# TODO: figure out how to build release and debug versions in separate
# directories for IDEs, then change --build-type to minimal
# and --layout to system
if(SCI_BOOST_LIBRARIES)
  foreach(lib ${SCI_BOOST_LIBRARIES})
    list(APPEND B2_ARGS "--with-${lib}")
  endforeach()

    if (BUILD_PYTHON)
      # --python-include does not appear to do anything (Boost 1.57)
      list(APPEND B2_ARGS
        "--with-python"
        "--python-lib=${SCI_PYTHON_LIBRARY_DIR}"
        "include=${SCI_PYTHON_INCLUDE}"
     )
    endif()
  else()
    # reasonable defaults for SCI software,
    # explicitly ignoring libraries not included in old CMake build
    # that are problematic on Windows
    list(APPEND B2_ARGS
      "--without-iostreams"
      "--without-locale"
      "--without-log"
      "--without-math")

    if(NOT BUILD_PYTHON)
      list(APPEND B2_ARGS
        "--without-python")
    endif()
endif()

add_custom_target(boost ALL
  COMMAND ${B2_COMMAND} ${B2_ARGS}
    DEPENDS bootstrap
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "Booststrap boost using: ${BOOTSTRAP_COMMAND}, with arg list: [${BOOTSTRAP_ARGS}]")
message(STATUS "Build boost using: ${B2_COMMAND}, with arg list: [${B2_ARGS}]")
