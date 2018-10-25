s1_add_library_internal(lexer
                        SOURCES
                        Lexer.cpp
                        ${S1_INC}/lexer/Lexer.h
                        ${S1_INC}/lexer/Token.h
                        ${S1_INC}/lexer/TokenLocation.h
                        ${S1_INC}/lexer/TokenType.h
                        DEPENDS generate_Diagnostics_lexer
                        LINK base)
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(lexer PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()

s1_add_boost_test(test_Lexer
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/t/LexerBasic.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/LexerComments.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/LexerIdentifiers.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/LexerKeywords.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/LexerNumeric.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/print_lexer_TokenType.h
                  DEPENDS lexer)
