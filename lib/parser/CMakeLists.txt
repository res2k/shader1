file(GLOB parser_sources *.h *.cpp ../include/parser/*.h)
add_library(parser ${parser_sources})
target_link_libraries(parser lexer)

if(CXXTEST_FOUND)
  file(GLOB parser_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.cpp)
  CXXTEST_ADD_TEST(test_Parser test_Parser.cpp
		   ${parser_test_sources})
  target_link_libraries(test_Parser parser)
endif()
