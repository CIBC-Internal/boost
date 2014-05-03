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

cmake_minimum_required(VERSION 2.8)
project(Boost)

if (APPLE)
  set(BOOST_TOOLSET "darwin")
elseif (WINDOWS)
  set(BOOST_TOOLSET "TODO")
elseif (UNIX)
  set(BOOST_TOOLSET "gcc")
else ()
  message(ERROR "Unsupported platform. Cannot detect Boost toolset.")
endif ()

option(BUILD_PYTHON "Build Python interface library" OFF)

if (BUILD_WITH_PYTHON)
  set(SCI_PYTHON_PATH "UNKNOWN")
  # should be major.minor
  set(SCI_PYTHON_VERSION "UNKNOWN")
  set(SCI_PYTHON_CONFIG "using python : ${SCI_PYTHON_VERSION} : ${SCI_PYTHON_PATH} ;" )
endif()

set(BOOST_BUILD_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
set(BOOST_BUILD_LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(BOOST_BUILD_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")

# all libraries built if empty
set(BOOST_LIBRARIES "")

set(BJAM_CONFIG_FILE_SOURCE project-config.jam.in)
set(BJAM_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/project-config.jam)
configure_file(${BJAM_CONFIG_FILE_SOURCE} ${BJAM_CONFIG_FILE} @ONLY NEWLINE_STYLE UNIX)

if (WINDOWS)
  set(BOOTSTRAP_COMMAND "TODO")
else ()
  # assuming *nix platforms that support bash scripts
  set(BOOTSTRAP_COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/bootstrap.sh")
endif()

set(BOOTSTRAP_ARGS "--prefix=${CMAKE_CURRENT_BINARY_DIR}")

add_custom_target(bootstrap ALL COMMAND ${BOOTSTRAP_COMMAND} ${BOOTSTRAP_ARGS} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_command(TARGET bootstrap POST_BUILD COMMAND rm -f project-config.jam WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(B2_COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/b2")
set(B2_ARGS "variant=release" "toolset=clang" "cxxflags=-std=c++11" "cxxflags=-stdlib=libc++" "link=static" "threading=multi" "linkflags=-stdlib=libc++" "--build-dir=${CMAKE_CURRENT_BINARY_DIR}" "--stagedir=${CMAKE_CURRENT_BINARY_DIR}/libs")
# run b2 with build-dir, stagedir
add_custom_target(boost ALL COMMAND ${B2_COMMAND} ${B2_ARGS} DEPENDS bootstrap WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
