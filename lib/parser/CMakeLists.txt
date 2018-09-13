set(parser_sources
    AstBuilder.cpp
    CommonSemanticsHandler.cpp
    Parser.cpp
    ${S1_INC}/parser/AstBuilder.h
    ${S1_INC}/parser/CommonSemanticsHandler.h
    ${S1_INC}/parser/Parser.h
    )
set(parser_sources_ast_h
    ${S1_INC}/parser/ast/Block.h
    ${S1_INC}/parser/ast/BlockStatement.h
    ${S1_INC}/parser/ast/BlockStatementFor.h
    ${S1_INC}/parser/ast/BlockStatementIf.h
    ${S1_INC}/parser/ast/BlockStatementWhile.h
    ${S1_INC}/parser/ast/BlockStatementReturn.h
    ${S1_INC}/parser/ast/Expr.h
    ${S1_INC}/parser/ast/ExprArrayElement.h
    ${S1_INC}/parser/ast/ExprAttribute.h
    ${S1_INC}/parser/ast/ExprBinary.h
    ${S1_INC}/parser/ast/ExprFunctionCall.h
    ${S1_INC}/parser/ast/ExprTernary.h
    ${S1_INC}/parser/ast/ExprUnary.h
    ${S1_INC}/parser/ast/ExprValue.h
    ${S1_INC}/parser/ast/forwarddecl.h
    ${S1_INC}/parser/ast/Identifier.h
    ${S1_INC}/parser/ast/Node.h
    ${S1_INC}/parser/ast/Program.h
    ${S1_INC}/parser/ast/ProgramStatement.h
    ${S1_INC}/parser/ast/ProgramStatementFunctionDecl.h
    ${S1_INC}/parser/ast/ProgramStatementTypedef.h
    ${S1_INC}/parser/ast/ProgramStatementVarsDecl.h
    ${S1_INC}/parser/ast/Type.h
    ${S1_INC}/parser/ast/TypeArray.h
    ${S1_INC}/parser/ast/Typedef.h
    ${S1_INC}/parser/ast/TypeIdentifier.h
    ${S1_INC}/parser/ast/TypeWellKnown.h
    ${S1_INC}/parser/ast/VarsDecl.h
    ${S1_INC}/parser/ast/VisitorBlockStatement.h
    ${S1_INC}/parser/ast/VisitorExpr.h
    ${S1_INC}/parser/ast/VisitorProgramStatement.h
    ${S1_INC}/parser/ast/VisitorType.h
    )
s1_add_library(parser
               SOURCES ${parser_sources} ${parser_sources_ast_h}
               LINK lexer)
source_group("ast Headers" FILES ${parser_sources_ast_h})
add_dependencies(parser generate_Diagnostics_parser)
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(parser PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()

if(CXXTEST_FOUND)
  file(GLOB parser_testlib_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.h)
  s1_add_library(parser_testlib STATIC SOURCES ${parser_testlib_sources} LINK headers_internal)
  set_property(TARGET parser_testlib PROPERTY EXCLUDE_FROM_ALL YES)

  set(parser_test_sources
      ${CMAKE_CURRENT_SOURCE_DIR}/t/AstBlock.h
      ${CMAKE_CURRENT_SOURCE_DIR}/t/AstBlockStrict.h
      ${CMAKE_CURRENT_SOURCE_DIR}/t/AstExpr.h
      ${CMAKE_CURRENT_SOURCE_DIR}/t/AstProgram.h
      ${CMAKE_CURRENT_SOURCE_DIR}/t/AstTypes.h
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

