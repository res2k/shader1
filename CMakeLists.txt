PROJECT(s1)
cmake_minimum_required(VERSION 2.6)

find_package(CxxTest)

if(CXXTEST_FOUND)
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()
endif()

find_program(ICU_CONFIG icu-config)
if(NOT ICU_CONFIG)
  message(FATAL_ERROR "ICU is required.")
endif()
message(STATUS "icu-config: ${ICU_CONFIG}")
set(ICU_FOUND ON)
execute_process(COMMAND ${ICU_CONFIG} --ldflags
		OUTPUT_VARIABLE ICU_LIBRARIES)
string(STRIP "${ICU_LIBRARIES}" ICU_LIBRARIES)
execute_process(COMMAND ${ICU_CONFIG} --ldflags-icuio
		OUTPUT_VARIABLE ICU_ICUIO_LIBRARIES)
string(STRIP "${ICU_ICUIO_LIBRARIES}" ICU_ICUIO_LIBRARIES)
execute_process(COMMAND ${ICU_CONFIG} --cppflags-searchpath
		OUTPUT_VARIABLE ICU_INCLUDE_DIRS)

SET(Boost_ADDITIONAL_VERSIONS "1.40" "1.40.0" "1.41" "1.41.0")
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
CxxCompilerFlagString (CXX_WARN_ALL "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_WARN_ALL}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CXX_AS_NEEDED_FLAG}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CXX_AS_NEEDED_FLAG}")

include_directories(include)

add_subdirectory(lib)
add_subdirectory(apps)
