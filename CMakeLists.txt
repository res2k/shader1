PROJECT(s1)
cmake_minimum_required(VERSION 2.6)

find_package(CxxTest)

if(CXXTEST_FOUND)
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()
endif()

add_subdirectory(lib)

ADD_EXECUTABLE(s1 main.cpp)

