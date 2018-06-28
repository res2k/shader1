set(intermediate_sources
    Builtin.h
    FunctionCallGlobalVarAugment.h
    PrependGlobalsInit.h
    CloningSequenceVisitor.cpp
    FunctionCallGlobalVarAugment.cpp
    FunctionGraph.cpp
    IntermediateGeneratorSemanticsHandler.cpp
    IntermediateGeneratorSemanticsHandler_SetupBuiltins.cpp
    PrependGlobalsInit.cpp
    Program.cpp
    ProgramFunction.cpp
    Sequence.cpp
    SequenceBuilder.cpp
    ${S1_INC}/intermediate/BasicType.h
    ${S1_INC}/intermediate/BuiltinFunction.h
    ${S1_INC}/intermediate/CloningSequenceVisitor.h
    ${S1_INC}/intermediate/FunctionGraph.h
    ${S1_INC}/intermediate/IntermediateGeneratorSemanticsHandler.h
    ${S1_INC}/intermediate/NullVisitor.h
    ${S1_INC}/intermediate/Program.h
    ${S1_INC}/intermediate/ProgramFunction.h
    ${S1_INC}/intermediate/Sequence.h
    ${S1_INC}/intermediate/SequenceBuilder.h
    ${S1_INC}/intermediate/SequenceVisitor.h
    ${S1_INC}/intermediate/forwarddecl.h
    )

include_directories(generator .)
add_subdirectory(generator)
add_subdirectory(SequenceOp)

s1_add_library_internal(intermediate
                        TARGET_LINK ${S1_TARGET_LINK}
                        SOURCES ${intermediate_sources}
                        DEPENDS generate_Diagnostics_intermediate
                        LINK intermediate_generator intermediate_seqop semantics parser)
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(intermediate_generator PRIVATE ${S1LIB_PDB_CXX_FLAGS})
  target_compile_options(intermediate_seqop PRIVATE ${S1LIB_PDB_CXX_FLAGS})
  target_compile_options(intermediate PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()

s1_add_boost_test(test_Intermediate
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/t/Decl.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/Expr.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/CommonChecks.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/print_TestSequenceVisitor_Operation.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/TestSequenceVisitor.h
                  DEPENDS intermediate)
set_property(TARGET test_Intermediate
             APPEND
             PROPERTY INCLUDE_DIRECTORIES
             ${CMAKE_CURRENT_SOURCE_DIR}/generator)

