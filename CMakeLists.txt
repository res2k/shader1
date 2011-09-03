PROJECT(s1)
cmake_minimum_required(VERSION 2.6)

find_package(CxxTest)

if(CXXTEST_FOUND)
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()
endif()

if(ICU_PREFIX)
  get_filename_component(ICU_PREFIX "${ICU_PREFIX}" ABSOLUTE)
  find_package_message(ICU_PREFIX "Using user-specified ICU_PREFIX: ${ICU_PREFIX}" ${ICU_PREFIX})
  set(ICU_FOUND ON)
  # FIXME: Those are the names for MSVC builds
  link_directories("${ICU_PREFIX}/lib")
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
endif()
include_directories(${ICU_INCLUDE_DIRS})

SET(Boost_ADDITIONAL_VERSIONS "1.47" "1.47.0")
find_package(Boost 1.35.0)
include_directories(${Boost_INCLUDE_DIRS})

include(CheckCXXCompilerFlag)
function(CxxCompilerFlagString flagvar flagstr)
  CHECK_CXX_COMPILER_FLAG("${flagstr}" ${flagvar}_FOUND)
  if(${flagvar}_FOUND)
    set(${flagvar} "${flagstr}" PARENT_SCOPE)
  endif()
endfunction()
CxxCompilerFlagString (CXX_AS_NEEDED_FLAG "-Wl,--as-needed")
if(NOT CMAKE_GENERATOR MATCHES "Visual Studio")
  # Annoying on MSVC; lots of warnings from inside Boost
  CxxCompilerFlagString (CXX_WARN_ALL "-Wall")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_WARN_ALL}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CXX_AS_NEEDED_FLAG}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CXX_AS_NEEDED_FLAG}")

if(CMAKE_GENERATOR MATCHES "Visual Studio")
  # Silence more annoyances
  add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

include_directories(include)

add_subdirectory(lib)
add_subdirectory(apps)
