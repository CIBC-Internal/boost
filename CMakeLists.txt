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

INCLUDE(CheckIncludeFileCXX)

########################################################################
# Set up compiler config

SET(HAVE_CLANG OFF)
SET(HAVE_GCC OFF)

IF(APPLE)
  # WARNING: CMake compiler variables may change!!!
  IF("${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ID}" MATCHES ".*[Cc]lang")
    SET(HAVE_CLANG ON)
  ENDIF()
ENDIF()

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
# TODO: TEMPORARY measure, until a Python externals library is added to SCIRun build
# Locate python libraries, headers

OPTION(BUILD_PYTHON "Build Python interface library" ON)

########################################################################
# Sources

SET(BOOST_ATOMIC_SRCS
  libs/atomic/src/lockpool.cpp
)

# TODO: go over flags and defines in Jamfile
SET(BOOST_CHRONO_SRCS
  libs/chrono/src/chrono.cpp
  libs/chrono/src/process_cpu_clocks.cpp
  libs/chrono/src/thread_clock.cpp
)

# TODO: context and coroutine new, not yet included in build

# TODO: assembler sources?
SET(BOOST_CONTEXT_SRCS)

IF(WIN32)
  SET(BOOST_COROUTINE_WIN32_SRCS
    libs/coroutine/src/detail/standard_stack_allocator_windows.cpp)
ENDIF()

SET(BOOST_COROUTINE_POSIX
    libs/coroutine/src/detail/standard_stack_allocator_posix.cpp
    libs/coroutine/src/detail/segmented_stack_allocator.cpp
)

# From coroutine library Boost Jamfile
SET_SOURCE_FILES_PROPERTIES(${BOOST_COROUTINE_POSIX} PROPERTIES
  COMPILE_DEFINITIONS BOOST_USE_SEGMENTED_STACKS
)

SET(BOOST_COROUTINE_SRCS
  ${BOOST_COROUTINE_WIN32_SRCS}
  ${BOOST_COROUTINE_POSIX}
   libs/coroutine/src/detail/coroutine_context.cpp
)

SET(BOOST_DATE_TIME_SRCS
  libs/date_time/src/gregorian/date_generators.cpp
  libs/date_time/src/gregorian/greg_month.cpp
  libs/date_time/src/gregorian/greg_weekday.cpp
  libs/date_time/src/gregorian/gregorian_types.cpp
)

# From signals library Boost Jamfile
SET_SOURCE_FILES_PROPERTIES(${BOOST_DATE_TIME_SRCS} PROPERTIES
  COMPILE_DEFINITIONS DATE_TIME_INLINE
)

SET(BOOST_EXCEPTION_SRCS
  libs/exception/src/clone_current_exception_non_intrusive.cpp
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

# TODO: verify that flags for older versions of MSVC are no longer needed...
SET(BOOST_GRAPH_SRCS
  libs/graph/src/read_graphviz_new.cpp
  libs/graph/src/graphml.cpp
)

# Requires MPI.
# Maybe enable later, if needed?
SET(BOOST_GRAPH_PARALLEL_SRCS )

# Requires compression libraries (zlib, bzip).
# Maybe enable later, if needed?
SET(BOOST_IOSTREAMS_SRCS )

# TODO: go over flags and defines in Jamfile and enable
IF(WIN32)
  SET(BOOST_LOCALE_WIN32_SRCS
    libs/locale/src/win32/collate.cpp
    libs/locale/src/win32/converter.cpp
    libs/locale/src/win32/lcid.cpp
    libs/locale/src/win32/numeric.cpp
    libs/locale/src/win32/win_backend.cpp
  )
ENDIF()

SET(BOOST_LOCALE_SRCS
  libs/locale/src/encoding/codepage.cpp
  libs/locale/src/icu/boundary.cpp
  libs/locale/src/icu/codecvt.cpp
  libs/locale/src/icu/collator.cpp
  libs/locale/src/icu/conversion.cpp
  libs/locale/src/icu/date_time.cpp
  libs/locale/src/icu/formatter.cpp
  libs/locale/src/icu/icu_backend.cpp
  libs/locale/src/icu/numeric.cpp
  libs/locale/src/icu/time_zone.cpp
  libs/locale/src/posix/codecvt.cpp
  libs/locale/src/posix/collate.cpp
  libs/locale/src/posix/converter.cpp
  libs/locale/src/posix/numeric.cpp
  libs/locale/src/posix/posix_backend.cpp
  libs/locale/src/shared/date_time.cpp
  libs/locale/src/shared/format.cpp
  libs/locale/src/shared/formatting.cpp
  libs/locale/src/shared/generator.cpp
  libs/locale/src/shared/ids.cpp
  libs/locale/src/shared/localization_backend.cpp
  libs/locale/src/shared/message.cpp
  libs/locale/src/shared/mo_lambda.cpp
  libs/locale/src/std/codecvt.cpp
  libs/locale/src/std/collate.cpp
  libs/locale/src/std/converter.cpp
  libs/locale/src/std/numeric.cpp
  libs/locale/src/std/std_backend.cpp
  libs/locale/src/util/codecvt_converter.cpp
  libs/locale/src/util/default_locale.cpp
  libs/locale/src/util/gregorian.cpp
  libs/locale/src/util/info.cpp
  libs/locale/src/util/locale_data.cpp
)

# TODO: go over flags and defines in Jamfile and enable
SET(BOOST_LOG_SRCS
  libs/log/src/attribute_name.cpp
  libs/log/src/attribute_set.cpp
  libs/log/src/attribute_value_set.cpp
  libs/log/src/code_conversion.cpp
  libs/log/src/core.cpp
  libs/log/src/date_time_format_parser.cpp
  libs/log/src/debug_output_backend.cpp
  libs/log/src/default_attribute_names.cpp
  libs/log/src/default_filter_factory.cpp
  libs/log/src/default_sink.cpp
  libs/log/src/dump.cpp
  libs/log/src/dump_avx2.cpp
  libs/log/src/dump_ssse3.cpp
  libs/log/src/event.cpp
  libs/log/src/event_log_backend.cpp
  libs/log/src/exceptions.cpp
  libs/log/src/filter_parser.cpp
  libs/log/src/format_parser.cpp
  libs/log/src/formatter_parser.cpp
  libs/log/src/global_logger_storage.cpp
  libs/log/src/init_from_settings.cpp
  libs/log/src/init_from_stream.cpp
  libs/log/src/light_rw_mutex.cpp
  libs/log/src/named_scope.cpp
  libs/log/src/named_scope_format_parser.cpp
  libs/log/src/once_block.cpp
  libs/log/src/parser_utils.cpp
  libs/log/src/process_id.cpp
  libs/log/src/process_name.cpp
  libs/log/src/record_ostream.cpp
  libs/log/src/settings_parser.cpp
  libs/log/src/severity_level.cpp
  libs/log/src/spirit_encoding.cpp
  libs/log/src/syslog_backend.cpp
  libs/log/src/text_file_backend.cpp
  libs/log/src/text_ostream_backend.cpp
  libs/log/src/thread_id.cpp
  libs/log/src/thread_specific.cpp
  libs/log/src/threadsafe_queue.cpp
  libs/log/src/timer.cpp
  libs/log/src/timestamp.cpp
  libs/log/src/trivial.cpp
  libs/log/src/unhandled_exception_count.cpp
)

# ignoring libs/math for now

# ignoring libs/mpi for now

# there is a compile flag for HPUX...
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

SET(BOOST_RANDOM_SRCS
  libs/random/src/random_device.cpp
)

# TODO: go over flags and defines in Jamfile
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


SET(BOOST_SIGNALS_SRCS
  libs/signals/src/connection.cpp
  libs/signals/src/named_slot_map.cpp
  libs/signals/src/signal_base.cpp
  libs/signals/src/slot.cpp
  libs/signals/src/trackable.cpp
)

# From signals library Boost Jamfile
SET_SOURCE_FILES_PROPERTIES(${BOOST_SIGNALS_SRCS} PROPERTIES
  COMPILE_DEFINITIONS BOOST_SIGNALS_NO_LIB=1
)

SET(BOOST_SYSTEM_SRCS
  libs/system/src/error_code.cpp
  libs/system/src/local_free_on_destruction.hpp
)

# TODO: go over flags and defines in Jamfile
SET(BOOST_TEST_SRCS
 libs/test/src/compiler_log_formatter.cpp
 libs/test/src/cpp_main.cpp
 libs/test/src/debug.cpp
 libs/test/src/exception_safety.cpp
 libs/test/src/execution_monitor.cpp
 libs/test/src/framework.cpp
 libs/test/src/interaction_based.cpp
 libs/test/src/logged_expectations.cpp
 libs/test/src/plain_report_formatter.cpp
 libs/test/src/progress_monitor.cpp
 libs/test/src/results_collector.cpp
 libs/test/src/results_reporter.cpp
 libs/test/src/test_main.cpp
 libs/test/src/test_tools.cpp
 libs/test/src/unit_test_log.cpp
 libs/test/src/unit_test_main.cpp
 libs/test/src/unit_test_monitor.cpp
 libs/test/src/unit_test_parameters.cpp
 libs/test/src/unit_test_suite.cpp
 libs/test/src/xml_log_formatter.cpp
 libs/test/src/xml_report_formatter.cpp
)

SET(BOOST_TIMER_SRCS
 libs/timer/src/auto_timers_construction.cpp
 libs/timer/src/cpu_timer.cpp
)

SET_SOURCE_FILES_PROPERTIES(${BOOST_TIMER_SRCS} PROPERTIES
  COMPILE_DEFINITIONS BOOST_TIMER_STATIC_LINK=1
)

# TODO: go over flags and defines in Jamfile
SET(BOOST_WAVE_SRCS
 libs/wave/src/instantiate_cpp_exprgrammar.cpp
 libs/wave/src/instantiate_cpp_grammar.cpp
 libs/wave/src/instantiate_cpp_literalgrs.cpp
 libs/wave/src/instantiate_defined_grammar.cpp
 libs/wave/src/instantiate_predef_macros.cpp
 libs/wave/src/instantiate_re2c_lexer.cpp
 libs/wave/src/instantiate_re2c_lexer_str.cpp
 libs/wave/src/token_ids.cpp
 libs/wave/src/wave_config_constant.cpp
 libs/wave/src/cpplexer/re2clex/aq.cpp
 libs/wave/src/cpplexer/re2clex/cpp_re.cpp
)

SET_SOURCE_FILES_PROPERTIES(${BOOST_WAVE_SRCS} PROPERTIES
  COMPILE_DEFINITIONS BOOST_THREAD_USE_LIB=1
)

IF(WIN32)
  SET_SOURCE_FILES_PROPERTIES(${BOOST_WAVE_SRCS} PROPERTIES
    COMPILE_DEFINITIONS _SCL_SECURE_NO_DEPRECATE
    COMPILE_DEFINITIONS _CRT_SECURE_NO_DEPRECATE
  )
ENDIF()

IF(BUILD_PYTHON)
  SET(BOOST_PYTHON_SRCS
    libs/python/src/dict.cpp
    libs/python/src/errors.cpp
    libs/python/src/exec.cpp
    libs/python/src/import.cpp
    libs/python/src/list.cpp
    libs/python/src/long.cpp
    libs/python/src/module.cpp
    libs/python/src/numeric.cpp
    libs/python/src/object_operators.cpp
    libs/python/src/object_protocol.cpp
    libs/python/src/slice.cpp
    libs/python/src/str.cpp
    libs/python/src/tuple.cpp
    libs/python/src/wrapper.cpp
    libs/python/src/object/class.cpp
    libs/python/src/object/enum.cpp
    libs/python/src/object/function.cpp
    libs/python/src/object/function_doc_signature.cpp
    libs/python/src/object/inheritance.cpp
    libs/python/src/object/iterator.cpp
    libs/python/src/object/life_support.cpp
    libs/python/src/object/pickle_support.cpp
    libs/python/src/object/stl_iterator.cpp
    libs/python/src/converter/arg_to_python_base.cpp
    libs/python/src/converter/builtin_converters.cpp
    libs/python/src/converter/from_python.cpp
    libs/python/src/converter/registry.cpp
    libs/python/src/converter/type_id.cpp
  )

  # From python library Boost Jamfile
  SET_SOURCE_FILES_PROPERTIES(${BOOST_PYTHON_SRCS} PROPERTIES
    COMPILE_DEFINITIONS BOOST_PYTHON_STATIC_LIB=1
    COMPILE_DEFINITIONS BOOST_PYTHON_SOURCE=1
    COMPILE_DEFINITIONS_DEBUG BOOST_DEBUG_PYTHON=1
  )
  SET(SCI_PYTHON_INCLUDE "" CACHE PATH "Python includes")
  INCLUDE_DIRECTORIES(${SCI_PYTHON_INCLUDE})
ENDIF()

# TODO: add compiler flags...
IF(WIN32)
  SET(BOOST_THREAD_SRCS
   libs/thread/src/win32/thread.cpp
   libs/thread/src/win32/tss_dll.cpp
   libs/thread/src/win32/tss_pe.cpp)
ELSE()
  SET(BOOST_THREAD_SRCS
   libs/thread/src/pthread/thread.cpp 
   libs/thread/src/pthread/once_atomic.cpp
   libs/thread/src/pthread/once.cpp)
ENDIF()

SET(BOOST_THREAD_SRCS
  ${BOOST_THREAD_SRCS}
  libs/thread/src/future.cpp
)

# From thread library Boost Jamfile
SET_SOURCE_FILES_PROPERTIES(${BOOST_THREAD_SRCS} PROPERTIES
  COMPILE_DEFINITIONS BOOST_SYSTEM_NO_DEPRECATED
  COMPILE_DEFINITIONS BOOST_THREAD_THROW_IF_PRECONDITION_NOT_SATISFIED
  COMPILE_DEFINITIONS BOOST_THREAD_DONT_PROVIDE_INTERRUPTIONS
  COMPILE_DEFINITIONS BOOST_THREAD_USE_LIB=1
)


IF(APPLE)
  IF(HAVE_CLANG)
    SET_SOURCE_FILES_PROPERTIES(${BOOST_THREAD_SRCS} PROPERTIES
      COMPILE_FLAGS "-Wno-delete-non-virtual-dtor"
    )
  ENDIF()
ELSEIF(WIN32 AND MSVC)
  SET_SOURCE_FILES_PROPERTIES(${BOOST_THREAD_SRCS} PROPERTIES
    COMPILE_FLAGS "/wd4512"
  )
ENDIF()


########################################################################
# Configure the library/libraries (Visual Studio 2010 (and greater?))

INCLUDE_DIRECTORIES(${SCI_BOOST_INCLUDE})

# From Boost Jamfiles
ADD_DEFINITIONS(-DBOOST_DATE_TIME_STATIC_LINK)
ADD_DEFINITIONS(-DBOOST_SYSTEM_STATIC_LINK=1)
ADD_DEFINITIONS(-DBOOST_All_STATIC_LINK=1)
# BOOST_ALL_NO_LIB needs to be defined in project that links this library
ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB=1)
ADD_DEFINITIONS(-DBOOST_FILESYSTEM_STATIC_LINK)
ADD_DEFINITIONS(-DBOOST_THREAD_BUILD_LIB)
ADD_DEFINITIONS(-DBOOST_THREAD_STATIC_LINK)
ADD_DEFINITIONS(-DBOOST_PYTHON_STATIC_LIB)

# TODO: check this
ADD_DEFINITIONS(-DZ_PREFIX)

ADD_LIBRARY(${SCI_BOOST_LIBRARY} STATIC
  ${BOOST_ATOMIC_SRCS}
  ${BOOST_CHRONO_SRCS}
  ${BOOST_DATE_TIME_SRCS}
  ${BOOST_EXCEPTION_SRCS}
  ${BOOST_FILESYSTEM_SRCS}
  ${BOOST_GRAPH_SRCS}
  ${BOOST_PROGRAM_OPTIONS_SRCS}
  ${BOOST_RANDOM_SRCS}
  ${BOOST_REGEX_SRCS}
  ${BOOST_SERIALIZATION_SRCS}
  ${BOOST_SIGNALS_SRCS}
  ${BOOST_SYSTEM_SRCS}
  ${BOOST_THREAD_SRCS}
  ${BOOST_TIMER_SRCS}
  ${BOOST_WAVE_SRCS}
  ${BOOST_PYTHON_SRCS}
)

IF(BUILD_PYTHON)
  TARGET_LINK_LIBRARIES(${SCI_BOOST_LIBRARY} ${SCI_PYTHON_LIBRARY})
ENDIF()

IF(ENABLE_PIC)
  SET_TARGET_PROPERTIES(${SCI_BOOST_LIBRARY}
      PROPERTIES
        COMPILE_FLAGS "-fPIC"
    )
ENDIF()
