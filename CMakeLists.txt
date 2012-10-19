#
#  For more information, please see: http://software.sci.utah.edu
# 
#  The MIT License
# 
#  Copyright (c) 2012 Scientific Computing and Imaging Institute,
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
#

# Minimal boost build set up as a CMake external project.
# Produces statically linked library.

########################################################################
# Boost project

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Boost)


########################################################################
# Compiler flags and definitions

# Better compiler check for OS X?
IF(CMAKE_COMPILER_IS_GNUCC)
  IF(UNIX)
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  ENDIF()

  IF(APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    SET(CMAKE_CXX_FLAGS_DEBUG "-Wshorten-64-to-32 ${CMAKE_CXX_FLAGS_DEBUG}")
  ENDIF()
ENDIF()


ADD_DEFINITIONS(-DBOOST_DATE_TIME_STATIC_LINK)
ADD_DEFINITIONS(-DBOOST_FILESYSTEM_STATIC_LINK)
ADD_DEFINITIONS(-DBOOST_THREAD_BUILD_LIB)
ADD_DEFINITIONS(-DZ_PREFIX)
ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)

# need: date_time filesystem regex system signals serialization thread program_options chrono 

########################################################################
# Sources
