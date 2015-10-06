file(GLOB intermediate_sources
     Builtin.h
     FunctionCallGlobalVarAugment.h
     PrependGlobalsInit.h
     CloningSequenceVisitor.cpp
     FunctionCallGlobalVarAugment.cpp
     IntermediateGeneratorSemanticsHandler.cpp
     IntermediateGeneratorSemanticsHandler_SetupBuiltins.cpp
     PrependGlobalsInit.cpp
     Program.cpp
     ProgramFunction.cpp
     Sequence.cpp
     SequenceBuilder.cpp
     ../../include/internal/intermediate/BasicType.h
     ../../include/internal/intermediate/BuiltinFunction.h
     ../../include/internal/intermediate/CloningSequenceVisitor.h
     ../../include/internal/intermediate/ErrorCode.h
     ../../include/internal/intermediate/Exception.h
     ../../include/internal/intermediate/IntermediateGeneratorSemanticsHandler.h
     ../../include/internal/intermediate/Program.h
     ../../include/internal/intermediate/ProgramFunction.h
     ../../include/internal/intermediate/Sequence.h
     ../../include/internal/intermediate/SequenceBuilder.h
     ../../include/internal/intermediate/SequenceVisitor.h
     ../../include/internal/intermediate/forwarddecl.h)

include_directories(generator .)
add_subdirectory(generator)
add_subdirectory(SequenceOp)

s1_add_library(intermediate
               SOURCES ${intermediate_sources}
               LINK intermediate_generator intermediate_seqop parser)
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

