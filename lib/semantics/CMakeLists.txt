set(semantics_sources
    Attribute.cpp
    CommonHandler.cpp
    CommonScope.cpp
    Type.cpp
    ${S1_INC}/semantics/CommonHandler.h
    ${S1_INC}/semantics/Attribute.h
    ${S1_INC}/semantics/CommonName.h
    ${S1_INC}/semantics/CommonScope.h
    ${S1_INC}/semantics/Type.h
    )
s1_add_library_internal(semantics
                        TARGET_LINK ${S1_TARGET_LINK}
                        SOURCES ${semantics_sources}
                        LINK base)
add_dependencies(semantics generate_Diagnostics_parser)
IF(S1LIB_PDB)
  target_compile_options(semantics PRIVATE ${S1LIB_PDB})
ENDIF()

file(GLOB semantics_testlib_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/t/lib/*.h)
s1_add_library_external(semantics_testlib STATIC SOURCES ${semantics_testlib_sources} LINK s1_headers_internal)
set_property(TARGET semantics_testlib PROPERTY EXCLUDE_FROM_ALL YES)

s1_add_boost_test(test_Semantics
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserBlock.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserBlockStrict.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserCommonAttrTypes.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserCommonTypeCompat.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserExpr.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserProgram.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserScope.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/ParserTypes.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/print_semantics_BaseType.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/print_semantics_SamplerType.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/TestParser.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/TestSemanticsHandler.h
                  DEPENDS semantics_testlib semantics parser)
