file(GLOB intermediate_sources
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

s1_add_library(intermediate
               SOURCES ${intermediate_sources}
               LINK intermediate_generator intermediate_seqop parser)
add_dependencies(intermediate generate_Diagnostics_intermediate)
IF(S1LIB_PDB)
  target_compile_options(intermediate_generator PRIVATE ${S1LIB_PDB})
  target_compile_options(intermediate_seqop PRIVATE ${S1LIB_PDB})
  target_compile_options(intermediate PRIVATE ${S1LIB_PDB})
ENDIF()

if(CXXTEST_FOUND)
  file(GLOB intermediate_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.h)
  s1_add_cxxtest(test_Intermediate
		 SOURCES ${intermediate_test_sources}
                 DEPENDS intermediate)
  set_property(TARGET test_Intermediate
               APPEND
               PROPERTY INCLUDE_DIRECTORIES
               ${CMAKE_CURRENT_SOURCE_DIR}/generator)
endif()

