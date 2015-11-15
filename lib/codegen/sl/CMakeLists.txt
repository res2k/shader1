set(codegen_sl_sources
    FunctionCodeGenerator.cpp
    ProgramCodeGenerator.cpp
    SequenceCodeGenerator.cpp
    Traits.cpp
    ${S1_INC}/codegen/sl/FunctionCodeGenerator.h
    ${S1_INC}/codegen/sl/Options.h
    ${S1_INC}/codegen/sl/ProgramCodeGenerator.h
    ${S1_INC}/codegen/sl/SequenceCodeGenerator.h
    ${S1_INC}/codegen/sl/Traits.h
)
s1_add_library(codegen_sl
               SOURCES ${codegen_sl_sources}
               LINK codegen_common)
IF(S1LIB_PDB)
  target_compile_options(codegen_sl PRIVATE ${S1LIB_PDB})
ENDIF()
