set(codegen_sl_sources
    SLFunctionCodeGenerator.cpp
    SLProgramCodeGenerator.cpp
    SLSequenceCodeGenerator.cpp
    SLTraits.cpp
    ${S1_INC}/codegen/sl/SLFunctionCodeGenerator.h
    ${S1_INC}/codegen/sl/SLOptions.h
    ${S1_INC}/codegen/sl/SLProgramCodeGenerator.h
    ${S1_INC}/codegen/sl/SLSequenceCodeGenerator.h
    ${S1_INC}/codegen/sl/SLTraits.h
)
s1_add_library_internal(codegen_sl
                        TARGET_LINK ${S1_TARGET_LINK}
                        SOURCES ${codegen_sl_sources}
                        LINK codegen_common)
add_dependencies(codegen_sl generate_Diagnostics_parser)
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(codegen_sl PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()
