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
execute_process(COMMAND ${ICU_CONFIG} --cppflags-searchpath
		OUTPUT_VARIABLE ICU_INCLUDE_DIRS)

include_directories(include)

add_subdirectory(lib)

ADD_EXECUTABLE(s1 main.cpp)

