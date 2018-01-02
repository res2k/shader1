set(parser_sources
    CommonSemanticsHandler.cpp
    Parser.cpp
    ${S1_INC}/parser/CommonSemanticsHandler.h
    ${S1_INC}/parser/Exception.h
    ${S1_INC}/parser/Parser.h
    ${S1_INC}/parser/SemanticsHandler.h
    )
s1_add_library(parser
               SOURCES ${parser_sources}
               LINK lexer)
add_dependencies(parser generate_Diagnostics_parser)
IF(S1LIB_PDB)
  target_compile_options(parser PRIVATE ${S1LIB_PDB})
ENDIF()

if(CXXTEST_FOUND)
  file(GLOB parser_testlib_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.h)
  s1_add_library(parser_testlib STATIC SOURCES ${parser_testlib_sources})
  set_property(TARGET parser_testlib PROPERTY EXCLUDE_FROM_ALL YES)

  set(parser_test_sources
      ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserBlock.h
      ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserBlockStrict.h
      ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserCommonAttrTypes.h
      ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserCommonTypeCompat.h
      ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserExpr.h
      ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserProgram.h
      ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserScope.h
      ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserTypes.h
      )
  s1_add_cxxtest(test_Parser
                 SOURCES ${parser_test_sources}
                 DEPENDS parser_testlib parser)
endif()

