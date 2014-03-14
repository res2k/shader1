file(GLOB parser_sources *.h *.cpp ../include/internal/parser/*.h)
add_library(parser STATIC ${parser_sources})
target_link_libraries(parser lexer)

if(CXXTEST_FOUND)
  file(GLOB parser_testlib_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.h)
  add_library(parser_testlib STATIC ${parser_testlib_sources})
  set_property(TARGET parser_testlib PROPERTY EXCLUDE_FROM_ALL YES)

  file(GLOB parser_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.h)
  CXXTEST_ADD_TEST(test_Parser test_Parser.cpp
		   ${parser_test_sources})
  target_link_libraries(test_Parser parser_testlib parser)
  set_property(TARGET test_Parser PROPERTY EXCLUDE_FROM_ALL YES)
  AutoBuildTest(test_Parser)
endif()

