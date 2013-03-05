set(compiler_sources
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
add_library(compiler ${compiler_sources})
target_link_libraries(compiler lexer intermediate codegen optimize splitter)
