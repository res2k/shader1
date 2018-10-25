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
s1_add_library_internal(parser
                        SOURCES ${parser_sources} ${parser_sources_ast_h}
                        LINK lexer)
source_group("ast Headers" FILES ${parser_sources_ast_h})
add_dependencies(parser generate_Diagnostics_parser)
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(parser PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()

file(GLOB parser_testlib_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.h)
s1_add_library_external(parser_testlib STATIC SOURCES ${parser_testlib_sources} LINK s1_headers_internal)
set_property(TARGET parser_testlib PROPERTY EXCLUDE_FROM_ALL YES)

s1_add_boost_test(test_Parser
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/t/AstBlock.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/AstBlockStrict.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/AstExpr.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/AstProgram.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/AstTypes.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserBlock.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserBlockStrict.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserCommonAttrTypes.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserCommonTypeCompat.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserExpr.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserProgram.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserScope.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserTypes.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/AstMacros.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/print_parser_Error.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/TestAstBuilder.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/TestParser.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/TestSemanticsHandler.h
                  DEPENDS parser_testlib parser)

