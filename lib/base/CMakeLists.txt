if(CXXTEST_FOUND)
  CXXTEST_ADD_TEST(test_base test_base.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/StupidTest.cpp)
endif()
