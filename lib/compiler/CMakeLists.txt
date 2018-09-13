set(compiler_sources
     Backend.cpp
     BackendGLSL.h
     BackendGLSL.cpp
     Compiler.cpp
     Options.cpp
     Program.cpp
     ProgramDiagnostics.cpp
     ${S1_INC}/compiler/Backend.h
     ${S1_INC}/compiler/BackendProgram.h
     ${S1_INC}/compiler/BackendProgramImpl.h
     ${S1_INC}/compiler/Compiler.h
     ${S1_INC}/compiler/Options.h
     ${S1_INC}/compiler/Program.h
     ${S1_INC}/compiler/ProgramDiagnostics.h
     )
s1_add_library(compiler
               SOURCES ${compiler_sources}
               LINK lexer intermediate codegen_glsl optimize splitter diagnostics)
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(compiler PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()
