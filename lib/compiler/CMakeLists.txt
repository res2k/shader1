set(compiler_sources
     Backend.cpp
     BackendCg.h
     BackendCg.cpp
     BackendGLSL.h
     BackendGLSL.cpp
     Compiler.cpp
     ErrorHandler.cpp
     ErrorHandler.h
     Options.cpp
     Program.cpp
     ${S1_INC}/compiler/Backend.h
     ${S1_INC}/compiler/Compiler.h
     ${S1_INC}/compiler/Options.h
     ${S1_INC}/compiler/Program.h
     )
s1_add_library(compiler
               SOURCES ${compiler_sources}
               LINK lexer intermediate codegen_cg codegen_glsl optimize splitter)
IF(S1LIB_PDB)
  target_compile_options(compiler PRIVATE ${S1LIB_PDB})
ENDIF()
