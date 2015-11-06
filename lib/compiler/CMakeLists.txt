set(compiler_sources
     Backend.cpp
     BackendCg.h
     BackendCg.cpp
     Compiler.cpp
     ErrorHandler.cpp
     ErrorHandler.h
     Options.cpp
     Program.cpp
     ../../include/internal/compiler/Backend.h
     ../../include/internal/compiler/Compiler.h
     ../../include/internal/compiler/Options.h
     ../../include/internal/compiler/Program.h
     )
s1_add_library(compiler
               SOURCES ${compiler_sources}
               LINK lexer intermediate codegen_cg optimize splitter)
IF(S1LIB_PDB)
  target_compile_options(compiler PRIVATE ${S1LIB_PDB})
ENDIF()
