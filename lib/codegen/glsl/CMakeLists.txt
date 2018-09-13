set(codegen_glsl_sources
    GLSLFunctionCodeGenerator.cpp
    GLSLGenerator.cpp
    GLSLOptions.cpp
    GLSLProgramCodeGenerator.cpp
    GLSLSequenceCodeGenerator.cpp
    GLSLTraits.cpp
    GLSLFunctionCodeGenerator.h
    GLSLProgramCodeGenerator.h
    GLSLSequenceCodeGenerator.h
    GLSLTraits.h
    ${S1_INC}/codegen/glsl/GLSLGenerator.h
    ${S1_INC}/codegen/glsl/GLSLOptions.h
)
s1_add_library(codegen_glsl
               SOURCES ${codegen_glsl_sources}
               LINK codegen_sl)
add_dependencies(codegen_glsl generate_Diagnostics_parser)
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(codegen_glsl PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()
