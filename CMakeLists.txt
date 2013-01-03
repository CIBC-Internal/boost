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

IF(NOT WIN32 AND NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
ENDIF(NOT WIN32 AND NOT CMAKE_BUILD_TYPE)

########################################################################
# Set the name of the library

SET(SCI_BOOST_LIBRARY boost CACHE INTERNAL "Boost library name." FORCE)

########################################################################
# Enable PIC for 64-bit Linux if necessary

IF(UNIX AND NOT APPLE AND NOT CYGWIN)
  SET(ENABLE_PIC ON)
ENDIF()

########################################################################
# Include directory

SET(SCI_BOOST_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Boost include directories." FORCE)

########################################################################
# Sources

# TODO: move all this into a macro

SET(BOOST_VERSION "-1_51")
SET(BOOST_ABI_DEBUG_TAB "-d")

IF(MSVC10)
  SET(BOOST_COMPILER "-vc100")
ELSEIF(MSVC11)
  SET(BOOST_COMPILER "-vc110")
ELSE()
  SET(BOOST_COMPILER "")
ENDIF()

SET(BOOST_MT "-mt")


SET(BOOST_CHRONO_SRCS
  libs/chrono/src/chrono.cpp
  libs/chrono/src/process_cpu_clocks.cpp
  libs/chrono/src/thread_clock.cpp
)

SET(BOOST_CHRONO_COMPONENT_NAME ${SCI_BOOST_LIBRARY}_chrono${BOOST_COMPILER}${BOOST_MT})

SET(BOOST_DATE_TIME_SRCS
  libs/date_time/src/gregorian/date_generators.cpp
  libs/date_time/src/gregorian/greg_month.cpp
  libs/date_time/src/gregorian/greg_weekday.cpp
  libs/date_time/src/gregorian/gregorian_types.cpp
  libs/date_time/src/posix_time/posix_time_types.cpp
)

SET(BOOST_DATE_TIME_COMPONENT_NAME ${SCI_BOOST_LIBRARY}_date_time${BOOST_COMPILER}${BOOST_MT})

# From signals library Boost Jamfile
SET_SOURCE_FILES_PROPERTIES(${BOOST_DATE_TIME_SRCS} PROPERTIES
  COMPILE_DEFINITIONS DATE_TIME_INLINE
  COMPILE_DEFINITIONS BOOST_DATE_TIME_STATIC_LINK
)

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

SET(BOOST_FILESYSTEM_COMPONENT_NAME ${SCI_BOOST_LIBRARY}_filesystem${BOOST_COMPILER}${BOOST_MT})

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

SET(BOOST_PROGRAM_OPTIONS_COMPONENT_NAME ${SCI_BOOST_LIBRARY}_program_options${BOOST_COMPILER}${BOOST_MT})

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

SET(BOOST_REGEX_COMPONENT_NAME ${SCI_BOOST_LIBRARY}_regex${BOOST_COMPILER}${BOOST_MT})

SET(BOOST_SERIALIZATION_SRCS
  libs/serialization/src/archive_exception.cpp
  libs/serialization/src/basic_archive.cpp
  libs/serialization/src/basic_iarchive.cpp
  libs/serialization/src/basic_iserializer.cpp
  libs/serialization/src/basic_oarchive.cpp
  libs/serialization/src/basic_oserializer.cpp
  libs/serialization/src/basic_pointer_iserializer.cpp
  libs/serialization/src/basic_pointer_oserializer.cpp
  libs/serialization/src/basic_serializer_map.cpp
  libs/serialization/src/basic_text_iprimitive.cpp
  libs/serialization/src/basic_text_oprimitive.cpp
  libs/serialization/src/basic_text_wiprimitive.cpp
  libs/serialization/src/basic_text_woprimitive.cpp
  libs/serialization/src/basic_xml_archive.cpp
  libs/serialization/src/binary_iarchive.cpp
  libs/serialization/src/binary_oarchive.cpp
  libs/serialization/src/binary_wiarchive.cpp
  libs/serialization/src/binary_woarchive.cpp
  libs/serialization/src/codecvt_null.cpp
  libs/serialization/src/extended_type_info.cpp
  libs/serialization/src/extended_type_info_no_rtti.cpp
  libs/serialization/src/extended_type_info_typeid.cpp
  libs/serialization/src/polymorphic_iarchive.cpp
  libs/serialization/src/polymorphic_oarchive.cpp
  libs/serialization/src/shared_ptr_helper.cpp
  libs/serialization/src/stl_port.cpp
  libs/serialization/src/text_iarchive.cpp
  libs/serialization/src/text_oarchive.cpp
  libs/serialization/src/text_wiarchive.cpp
  libs/serialization/src/text_woarchive.cpp
  libs/serialization/src/utf8_codecvt_facet.cpp
  libs/serialization/src/void_cast.cpp
  libs/serialization/src/xml_archive_exception.cpp
  libs/serialization/src/xml_grammar.cpp
  libs/serialization/src/xml_iarchive.cpp
  libs/serialization/src/xml_oarchive.cpp
  libs/serialization/src/xml_wgrammar.cpp
  libs/serialization/src/xml_wiarchive.cpp
  libs/serialization/src/xml_woarchive.cpp
)

SET(BOOST_SERIALIZATION_COMPONENT_NAME ${SCI_BOOST_LIBRARY}_serialization${BOOST_COMPILER}${BOOST_MT})

SET(BOOST_SIGNALS_SRCS
  libs/signals/src/connection.cpp
  libs/signals/src/named_slot_map.cpp
  libs/signals/src/signal_base.cpp
  libs/signals/src/slot.cpp
  libs/signals/src/trackable.cpp
)

# From signals library Boost Jamfile
SET_SOURCE_FILES_PROPERTIES(${BOOST_THREAD_SRCS} PROPERTIES
  COMPILE_DEFINITIONS BOOST_SIGNALS_NO_LIB=1
)

SET(BOOST_SIGNALS_COMPONENT_NAME ${SCI_BOOST_LIBRARY}_signals${BOOST_COMPILER}${BOOST_MT})

SET(BOOST_SYSTEM_SRCS
  libs/system/src/error_code.cpp
  libs/system/src/local_free_on_destruction.hpp
)

SET(BOOST_SYSTEM_COMPONENT_NAME ${SCI_BOOST_LIBRARY}_system${BOOST_COMPILER}${BOOST_MT})

# TODO: check this!!!
IF(WIN32)
  SET(BOOST_THREAD_SRCS
   libs/thread/src/win32/thread.cpp
   libs/thread/src/win32/tss_dll.cpp
   libs/thread/src/win32/tss_pe.cpp)
ELSE()
  SET(BOOST_THREAD_SRCS
   libs/thread/src/pthread/thread.cpp 
   libs/thread/src/pthread/once.cpp)
ENDIF()

SET(BOOST_THREAD_SRCS
  ${BOOST_THREAD_SRCS}
  libs/thread/src/future.cpp
)

# From thread library Boost Jamfile
SET_SOURCE_FILES_PROPERTIES(${BOOST_THREAD_SRCS} PROPERTIES
  COMPILE_DEFINITIONS BOOST_SYSTEM_NO_DEPRECATED
  COMPILE_DEFINITIONS BOOST_THREAD_USE_LIB=1
)

SET(BOOST_THREAD_COMPONENT_NAME ${SCI_BOOST_LIBRARY}_thread${BOOST_COMPILER}${BOOST_MT})

########################################################################
# Configure the library/libraries (Visual Studio 2010 (and greater?))

INCLUDE_DIRECTORIES(${SCI_BOOST_INCLUDE})

# From Boost Jamfiles
ADD_DEFINITIONS(-DBOOST_DATE_TIME_STATIC_LINK)
ADD_DEFINITIONS(-DBOOST_SYSTEM_STATIC_LINK=1)
ADD_DEFINITIONS(-DBOOST_All_STATIC_LINK=1)
ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB=1)
ADD_DEFINITIONS(-DBOOST_FILESYSTEM_STATIC_LINK)
ADD_DEFINITIONS(-DBOOST_THREAD_BUILD_LIB)

# TODO: check this
ADD_DEFINITIONS(-DZ_PREFIX)

# TODO: Windows use static runtime (ABI tag -s)? Check jam config files...
#
# TODO: Create a macro for adding Windows libraries

# see http://www.boost.org/doc/libs/1_51_0/more/getting_started/windows.html#expected-build-output
IF(WIN32 AND MSVC)
  # always building multithreaded currently

  ADD_LIBRARY(${BOOST_CHRONO_COMPONENT_NAME}
    ${BOOST_CHRONO_SRCS}
  )

  SET_TARGET_PROPERTIES(${BOOST_CHRONO_COMPONENT_NAME}
    PROPERTIES
      OUTPUT_NAME_RELEASE ${BOOST_CHRONO_COMPONENT_NAME}${BOOST_VERSION}
      OUTPUT_NAME_DEBUG ${BOOST_CHRONO_COMPONENT_NAME}${BOOST_ABI_DEBUG_TAG}${BOOST_VERSION}
  )

  ADD_LIBRARY(${BOOST_DATE_TIME_COMPONENT_NAME}
    ${BOOST_DATE_TIME_SRCS}
  )

  SET_TARGET_PROPERTIES(${BOOST_DATE_TIME_COMPONENT_NAME}
    PROPERTIES
      OUTPUT_NAME_RELEASE ${BOOST_DATE_TIME_COMPONENT_NAME}${BOOST_VERSION}
      OUTPUT_NAME_DEBUG ${BOOST_DATE_TIME_COMPONENT_NAME}${BOOST_ABI_DEBUG_TAG}${BOOST_VERSION}
  )

  ADD_LIBRARY(${BOOST_FILESYTEM_COMPONENT_NAME}
    ${BOOST_FILESYTEM_SRCS}
  )

  SET_TARGET_PROPERTIES(${BOOST_FILESYTEM_COMPONENT_NAME}
    PROPERTIES
      OUTPUT_NAME_RELEASE ${BOOST_FILESYTEM_COMPONENT_NAME}${BOOST_VERSION}
      OUTPUT_NAME_DEBUG ${BOOST_FILESYTEM_COMPONENT_NAME}${BOOST_ABI_DEBUG_TAG}${BOOST_VERSION}
  )

  ADD_LIBRARY(${BOOST_PROGRAM_OPTIONS_COMPONENT_NAME}
    ${BOOST_PROGRAM_OPTIONS_SRCS}
  )

  SET_TARGET_PROPERTIES(${BOOST_PROGRAM_OPTIONS_COMPONENT_NAME}
    PROPERTIES
      OUTPUT_NAME_RELEASE ${BOOST_PROGRAM_OPTIONS_COMPONENT_NAME}${BOOST_VERSION}
      OUTPUT_NAME_DEBUG ${BOOST_PROGRAM_OPTIONS_COMPONENT_NAME}${BOOST_ABI_DEBUG_TAG}${BOOST_VERSION}
  )

  ADD_LIBRARY(${BOOST_REGEX_COMPONENT_NAME}
    ${BOOST_REGEX_SRCS}
  )

  SET_TARGET_PROPERTIES(${BOOST_REGEX_COMPONENT_NAME}
    PROPERTIES
      OUTPUT_NAME_RELEASE ${BOOST_REGEX_COMPONENT_NAME}${BOOST_VERSION}
      OUTPUT_NAME_DEBUG ${BOOST_REGEX_COMPONENT_NAME}${BOOST_ABI_DEBUG_TAG}${BOOST_VERSION}
  )

  ADD_LIBRARY(${BOOST_SERIALIZATION_COMPONENT_NAME}
    ${BOOST_SERIALIZATION_SRCS}
  )
  
  SET_TARGET_PROPERTIES(${BOOST_SERIALIZATION_COMPONENT_NAME}
    PROPERTIES
      OUTPUT_NAME_RELEASE ${BOOST_SERIALIZATION_COMPONENT_NAME}${BOOST_VERSION}
      OUTPUT_NAME_DEBUG ${BOOST_SERIALIZATION_COMPONENT_NAME}${BOOST_ABI_DEBUG_TAG}${BOOST_VERSION}
  )

  ADD_LIBRARY(${BOOST_SIGNALS_COMPONENT_NAME}
    ${BOOST_SIGNALS_SRCS}
  )

  SET_TARGET_PROPERTIES(${BOOST_SIGNALS_COMPONENT_NAME}
    PROPERTIES
      OUTPUT_NAME_RELEASE ${BOOST_SIGNALS_COMPONENT_NAME}${BOOST_VERSION}
      OUTPUT_NAME_DEBUG ${BOOST_SIGNALS_COMPONENT_NAME}${BOOST_ABI_DEBUG_TAG}${BOOST_VERSION}
  )

  ADD_LIBRARY(${BOOST_SYSTEM_COMPONENT_NAME}
    ${BOOST_SYSTEM_SRCS}
  )

  SET_TARGET_PROPERTIES(${BOOST_SYSTEM_COMPONENT_NAME}
    PROPERTIES
      OUTPUT_NAME_RELEASE ${BOOST_SYSTEM_COMPONENT_NAME}${BOOST_VERSION}
      OUTPUT_NAME_DEBUG ${BOOST_SYSTEM_COMPONENT_NAME}${BOOST_ABI_DEBUG_TAG}${BOOST_VERSION}
  )

  ADD_LIBRARY(${BOOST_THREAD_COMPONENT_NAME}
    ${BOOST_THREAD_SRCS}
  )

  SET_TARGET_PROPERTIES(${BOOST_THREAD_COMPONENT_NAME}
    PROPERTIES
      OUTPUT_NAME_RELEASE ${BOOST_THREAD_COMPONENT_NAME}${BOOST_VERSION}
      OUTPUT_NAME_DEBUG ${BOOST_THREAD_COMPONENT_NAME}${BOOST_ABI_DEBUG_TAG}${BOOST_VERSION}
  )

ELSE()
  ADD_LIBRARY(${SCI_BOOST_LIBRARY} STATIC
    ${BOOST_CHRONO_SRCS}
    ${BOOST_DATE_TIME_SRCS}
    ${BOOST_FILESYSTEM_SRCS}
    ${BOOST_PROGRAM_OPTIONS_SRCS}
    ${BOOST_REGEX_SRCS}
    ${BOOST_SERIALIZATION_SRCS}
    ${BOOST_SIGNALS_SRCS}
    ${BOOST_SYSTEM_SRCS}
    ${BOOST_THREAD_SRCS}
  )

  IF(ENABLE_PIC)
    SET_TARGET_PROPERTIES(${SCI_BOOST_LIBRARY}
      PROPERTIES
        COMPILE_FLAGS "-fPIC"
    )
  ENDIF()
ENDIF()
