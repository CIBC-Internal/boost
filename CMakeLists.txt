#  For more information, please see: http://software.sci.utah.edu
# 
#  The MIT License
# 
#  Copyright (c) 2014 Scientific Computing and Imaging Institute,
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
  set(BOOST_TOOLSET "gcc")
else()
  message(ERROR "Unsupported platform. Cannot detect Boost toolset.")
endif()

option(BUILD_PYTHON "Build Python interface library" OFF)
option(FORCE_64BIT_BUILD "Force build to be 64 bit" OFF)

if(NOT BUILD_PYTHON)
  set(SCI_PYTHON_PATH "UNKNOWN")
  # should be major.minor
  set(SCI_PYTHON_VERSION "UNKNOWN")
else()
  set(SCI_PYTHON_CONFIG "using python : ${SCI_PYTHON_VERSION} : ${SCI_PYTHON_PATH} ;" )
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

# all libraries built if empty
set(BOOST_LIBRARIES "")

set(BJAM_CONFIG_FILE_SOURCE project-config.jam.in)
set(BJAM_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/project-config.jam)
configure_file(${BJAM_CONFIG_FILE_SOURCE} ${BJAM_CONFIG_FILE} @ONLY NEWLINE_STYLE UNIX)

if(WIN32)
  file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bootstrap.bat" WIN32_BOOTSTRAP_COMMAND)
  set(BOOTSTRAP_COMMAND "${WIN32_BOOTSTRAP_COMMAND}")
else()
  # assuming *nix platforms that support bash scripts
  set(BOOTSTRAP_COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/bootstrap.sh")
endif()

# not sure if this has any effect on Windows, but not rejected either
set(BOOTSTRAP_ARGS "--prefix=${CMAKE_CURRENT_BINARY_DIR}")

add_custom_target(bootstrap ALL
  COMMAND ${BOOTSTRAP_COMMAND} ${BOOTSTRAP_ARGS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(TARGET bootstrap
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E remove project-config.jam
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


set(B2_ARGS "variant=${VARIANT}" "link=static" "threading=multi")

if(FORCE_64BIT_BUILD)
  list(APPEND B2_ARGS "address-model=64")
endif()

if(APPLE)
  list(APPEND B2_ARGS "toolset=clang" "cxxflags=-std=c++11" "cxxflags=-stdlib=libc++" "linkflags=-stdlib=libc++")
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
  list(APPEND B2_ARGS "cxxflags=-fPIC")
endif()

if(NOT BUILD_PYTHON)
  list(APPEND B2_ARGS
         "--without-python")
endif()

# run b2 with build-dir, stagedir
# not currently supporting zlib, bzip2
# explicitly ignoring libraries not included in old CMake build
# that are problematic on Windows

# TODO: figure out how to build release and debug versions in separate
# directories for IDEs, then change --build-type to minimal
# and --layout to system
list(APPEND B2_ARGS
        "-sNO_ZLIB=1"
        "-sNO_BZIP2=1"
        "--without-iostreams"
        "--without-locale"
        "--without-log"
        "--without-math"
        "--build-type=complete"
        "--build-dir=${CMAKE_CURRENT_BINARY_DIR}"
        "--stagedir=${CMAKE_CURRENT_BINARY_DIR}"
        "--layout=tagged"
        "stage")

add_custom_target(boost ALL
  COMMAND ${B2_COMMAND} ${B2_ARGS}
    DEPENDS bootstrap
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "Build boost using: ${B2_COMMAND}, with arg list: [${B2_ARGS}]")
