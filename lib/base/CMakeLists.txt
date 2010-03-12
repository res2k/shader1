file(GLOB base_sources *.h *.cpp ../include/base/*.h)
add_library(base ${base_sources})
target_link_libraries(base ${ICU_LIBRARIES})

if(CXXTEST_FOUND)
  CXXTEST_ADD_TEST(test_UnicodeStream test_UnicodeStream.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/UnicodeStream.cpp)
  target_link_libraries(test_UnicodeStream base)
endif()
