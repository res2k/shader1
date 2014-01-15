set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/c_flag_overrides.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cxx_flag_overrides.cmake)

PROJECT(s1)
cmake_minimum_required(VERSION 2.6)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

find_package(CxxTest)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/libutils.cmake)

if(CXXTEST_FOUND)
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()
endif()

if(ICU_PREFIX)
  get_filename_component(ICU_PREFIX "${ICU_PREFIX}" ABSOLUTE)
  find_package_message(ICU_PREFIX "Using user-specified ICU_PREFIX: ${ICU_PREFIX}" ${ICU_PREFIX})
  set(ICU_FOUND ON)
  # FIXME: Those are the names for MSVC builds
  if(CMAKE_GENERATOR MATCHES "Win64")
	link_directories("${ICU_PREFIX}/lib64")
  else()
	link_directories("${ICU_PREFIX}/lib")
  endif()
  set(ICU_LIBRARIES icuin icuuc icudt)
  set(ICU_ICUIO_LIBRARIES icuio)
  set(ICU_INCLUDE_DIRS "${ICU_PREFIX}/include")
else()
  find_program(ICU_CONFIG icu-config)
  if(NOT ICU_CONFIG)
    message(FATAL_ERROR "ICU is required. Make sure 'icu-config' is in PATH or set the cmake var ICU_PREFIX.")
  endif()
  find_package_message(ICU_CONFIG "icu-config: ${ICU_CONFIG}" ${ICU_CONFIG})
  set(ICU_FOUND ON)
  execute_process(COMMAND ${ICU_CONFIG} --ldflags
		  OUTPUT_VARIABLE ICU_LIBRARIES)
  string(STRIP "${ICU_LIBRARIES}" ICU_LIBRARIES)
  execute_process(COMMAND ${ICU_CONFIG} --ldflags-icuio
		  OUTPUT_VARIABLE ICU_ICUIO_LIBRARIES)
  string(STRIP "${ICU_ICUIO_LIBRARIES}" ICU_ICUIO_LIBRARIES)
  execute_process(COMMAND ${ICU_CONFIG} --cppflags-searchpath
		  OUTPUT_VARIABLE ICU_INCLUDE_DIRS)
  string(REGEX REPLACE "^-I" "" ICU_INCLUDE_DIRS "${ICU_INCLUDE_DIRS}")
endif()
include_directories(${ICU_INCLUDE_DIRS})

find_package(Boost 1.53.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include(CheckCXXCompilerFlag)
function(CxxCompilerFlagString flagvar flagstr)
  CHECK_CXX_COMPILER_FLAG("${flagstr}" ${flagvar}_FOUND)
  if(${flagvar}_FOUND)
    set(${flagvar} "${flagstr}" PARENT_SCOPE)
  endif()
endfunction()
function(CxxCompilerFlagStringAppend flagvar flagstr)
  string(TOUPPER "${flagstr}" auto_flag_var)
  string(REGEX REPLACE "[^A-Z0-9]" "_" auto_flag_var "${auto_flag_var}")
  CxxCompilerFlagString (CXX_${auto_flag_var} "${flagstr}")
  if(NOT "${CXX_${auto_flag_var}}" EQUAL "")
    set(${flagvar} "${${flagvar}} ${CXX_${auto_flag_var}}" PARENT_SCOPE)
  endif()
endfunction()

if(NOT MSVC)
  # Annoying on MSVC; lots of warnings from inside Boost
  CxxCompilerFlagStringAppend (CMAKE_CXX_FLAGS_DEBUG "-Wall")
  CxxCompilerFlagStringAppend (CMAKE_CXX_FLAGS_DEBUG "-Wno-unused-local-typedefs")
endif()
if(CMAKE_COMPILER_IS_GNUCXX)
  CxxCompilerFlagStringAppend (CMAKE_CXX_FLAGS_DEBUG "-fvisibility-inlines-hidden -fvisibility=hidden")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_WARN_ALL} ${CXX_VISIBILITY_HIDDEN}")

CxxCompilerFlagString (CXX_AS_NEEDED_FLAG "-Wl,--as-needed")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CXX_AS_NEEDED_FLAG}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CXX_AS_NEEDED_FLAG}")

CHECK_CXX_SOURCE_COMPILES(
  "int main() { void* p = nullptr; return 0; }"
  CXX_HAS_NULLPTR)
if(NOT CXX_HAS_NULLPTR)
endif()

# Build shared libs by default
set(S1_BUILD_SHARED ON CACHE BOOL "Whether to build Shader1 as a shared library")
set(BUILD_SHARED_LIBS ${S1_BUILD_SHARED})

if(MSVC)
  # Automatically make use of 'secure' CRT funtions
  add_definitions("-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1")
  add_definitions("-D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT=1")
  # Silence remaining annoyances
  add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

# Build everything with -fPIC to allow linking into shared libs
if(UNIX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC")
  set(WITH_PIC YES)
endif()

# Configuration settings only relevant for building
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/internal/base)
configure_file(include/internal/base/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/internal/base/config.h)
# Configuration settings exposed to clients
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/s1)
configure_file(include/s1/s1config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/s1/s1config.h)

add_custom_target(build_tests)
function(AutoBuildTest test_name)
  add_test(test_build_${test_name} "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${test_name})
  set_tests_properties(${test_name} PROPERTIES DEPENDS test_build_${test_name})
  add_dependencies(build_tests ${test_name})
endfunction()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/internal)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
include_directories(include/internal)
include_directories(include)

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(apps)

add_subdirectory(doc)
