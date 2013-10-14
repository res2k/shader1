file(GLOB lexer_sources *.h *.cpp ../include/internal/lexer/*.h)
add_library(lexer STATIC ${lexer_sources})
target_link_libraries(lexer base)

if(CXXTEST_FOUND)
  file(GLOB lexer_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.cpp)
  CXXTEST_ADD_TEST(test_Lexer test_Lexer.cpp
		   ${lexer_test_sources})
  target_link_libraries(test_Lexer lexer)
  set_property(TARGET test_Lexer PROPERTY EXCLUDE_FROM_ALL YES)
  AutoBuildTest(test_Lexer)
endif()
