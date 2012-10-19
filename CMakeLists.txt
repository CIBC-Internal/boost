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
# Set the name of the library

SET(SCI_BOOST_LIBRARY boost CACHE INTERNAL "Boost library name." FORCE)

########################################################################
# Enable PIC for 64-bit Linux if necessary

IF(BUILD_SHARED_LIBS AND UNIX AND NOT APPLE AND NOT CYGWIN)
  SET(ENABLE_PIC ON)
ENDIF()

########################################################################
# Include directory

SET(SCI_BOOST_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Boost include directories." FORCE)

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

IF(ENABLE_PIC)
  SET_TARGET_PROPERTIES(${SCI_BOOST_LIBRARY}
                        PROPERTIES
                          COMPILE_FLAGS "-fPIC"
  )
ENDIF()

########################################################################
# Sources

# need: date_time filesystem regex system signals serialization thread program_options chrono 

SET(BOOST_CHRONO_SRCS )

SET(BOOST_DATE_TIME_SRCS )

SET(BOOST_FILESYSTEM_SRCS
  libs/filesystem/src/codecvt_error_category.cpp
  libs/filesystem/src/operations.cpp
  libs/filesystem/src/path.cpp
  libs/filesystem/src/path_traits.cpp
  libs/filesystem/src/portability.cpp
  libs/filesystem/src/unique_path.cpp
  libs/filesystem/src/utf8_codecvt_facet.cpp
  libs/filesystem/src/windows_file_codecvt.cpp
)

SET(BOOST_FILESYSTEM_HEADERS
  libs/filesystem/src/windows_file_codecvt.hpp
)

SET(BOOST_PROGRAM_OPTIONS_SRCS
  libs/program_options/src/cmdline.cpp 
  libs/program_options/src/config_file.cpp 
  libs/program_options/src/convert.cpp 
  libs/program_options/src/options_description.cpp 
  libs/program_options/src/parsers.cpp 
  libs/program_options/src/positional_options.cpp 
  libs/program_options/src/split.cpp 
  libs/program_options/src/utf8_codecvt_facet.cpp 
  libs/program_options/src/value_semantic.cpp 
  libs/program_options/src/variables_map.cpp 
  libs/program_options/src/winmain.cpp
)

SET(BOOST_REGEX_SRCS
  libs/regex/src/c_regex_traits.cpp
  libs/regex/src/cpp_regex_traits.cpp
  libs/regex/src/cregex.cpp
  libs/regex/src/fileiter.cpp
  libs/regex/src/icu.cpp
  libs/regex/src/instances.cpp
  libs/regex/src/posix_api.cpp
  libs/regex/src/regex.cpp
  libs/regex/src/regex_debug.cpp
  libs/regex/src/regex_raw_buffer.cpp
  libs/regex/src/regex_traits_defaults.cpp
  libs/regex/src/static_mutex.cpp
  libs/regex/src/usinstances.cpp   
  libs/regex/src/w32_regex_traits.cpp
  libs/regex/src/wc_regex_traits.cpp
  libs/regex/src/wide_posix_api.cpp
  libs/regex/src/winstances.cpp 
)

SET(BOOST_SERIALIZATION_SRCS )

SET(BOOST_SIGNALS_SRCS )

SET(BOOST_SYSTEM_SRCS )

SET(BOOST_THREAD_SRCS )

########################################################################
# Configure the library

ADD_LIBRARY(
  ${SCI_BOOST_LIBRARY} STATIC
    ${BOOST_FILESYSTEM_SRCS}
    ${BOOST_PROGRAM_OPTIONS_SRCS}
    ${BOOST_REGEX_SRCS}
)
