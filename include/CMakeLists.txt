add_subdirectory(s1)

if(CXXTEST_FOUND)
  CXXTEST_ADD_TEST(test_api_Library test_api_Library.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiLibrary.cpp)
  target_link_libraries(test_api_Library base)

  add_custom_target(test_api DEPENDS test_api_Library)
endif()
