set(codegen_cg_sources
    CgGenerator.cpp
    CgOptions.cpp
    FunctionCodeGenerator.cpp
    ProgramCodeGenerator.cpp
    SequenceCodeGenerator.cpp
    FunctionCodeGenerator.h
    ProgramCodeGenerator.h
    SequenceCodeGenerator.h
    ../../../include/internal/codegen/cg/CgGenerator.h
    ../../../include/internal/codegen/cg/CgOptions.h
)
s1_add_library(codegen_cg
               SOURCES ${codegen_cg_sources}
               LINK codegen_common)
IF(S1LIB_PDB)
  target_compile_options(codegen_cg PRIVATE ${S1LIB_PDB})
ENDIF()
