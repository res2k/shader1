file(GLOB lexer_sources *.h *.cpp ../include/internal/lexer/*.h)
s1_add_library(lexer
               SOURCES ${lexer_sources}
               LINK base)
IF(S1LIB_PDB)
  target_compile_options(lexer PRIVATE ${S1LIB_PDB})
ENDIF()

if(CXXTEST_FOUND)
  file(GLOB lexer_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.h)
  s1_add_cxxtest(test_Lexer
		 SOURCES ${lexer_test_sources}
                 DEPENDS lexer)
endif()
