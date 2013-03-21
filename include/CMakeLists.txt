add_subdirectory(s1)

if(CXXTEST_FOUND)
  file(GLOB api_testlib_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.h)
  add_library(api_testlib ${api_testlib_sources})
  set_property(TARGET api_testlib PROPERTY COMPILE_DEFINITIONS S1_BUILD)

  file(GLOB api_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.cpp)
  CXXTEST_ADD_TEST(test_Api test_Api.cpp
                   ${api_test_sources})
  target_link_libraries(test_Api api_testlib s1)
endif()
