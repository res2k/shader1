set(codegen_cg_sources
    CgGenerator.cpp
    CgOptions.cpp
    CgTraits.cpp
    CgFunctionCodeGenerator.cpp
    CgProgramCodeGenerator.cpp
    CgSequenceCodeGenerator.cpp
    CgTraits.h
    CgFunctionCodeGenerator.h
    CgProgramCodeGenerator.h
    CgSequenceCodeGenerator.h
    ${S1_INC}/codegen/cg/CgGenerator.h
    ${S1_INC}/codegen/cg/CgOptions.h
)
s1_add_library(codegen_cg
               SOURCES ${codegen_cg_sources}
               LINK codegen_sl)
IF(S1LIB_PDB)
  target_compile_options(codegen_cg PRIVATE ${S1LIB_PDB})
ENDIF()
