set(codegen_glsl_sources
    FunctionCodeGenerator.cpp
    Generator.cpp
    Options.cpp
    ProgramCodeGenerator.cpp
    SequenceCodeGenerator.cpp
    Traits.cpp
    FunctionCodeGenerator.h
    ProgramCodeGenerator.h
    SequenceCodeGenerator.h
    Traits.h
    ${S1_INC}/codegen/glsl/Generator.h
    ${S1_INC}/codegen/glsl/Options.h
)
s1_add_library(codegen_glsl
               SOURCES ${codegen_glsl_sources}
               LINK codegen_sl)
IF(S1LIB_PDB)
  target_compile_options(codegen_glsl PRIVATE ${S1LIB_PDB})
ENDIF()
