add_subdirectory(s1)

if(CXXTEST_FOUND)
  CXXTEST_ADD_TEST(test_api_Object test_api_Object.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/ApiObject.cpp)
  target_link_libraries(test_api_Object base)

  add_custom_target(test_api DEPENDS test_api_Object)
endif()
