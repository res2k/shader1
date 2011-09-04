set(compiler_sources
     BackendCg.h
     BackendCg.cpp
     Compiler.cpp
     DeprotectCtor.h
     ErrorHandler.cpp
     ErrorHandler.h
     Options.cpp
     Program.cpp
     ../../include/compiler/Backend.h
     ../../include/compiler/Compiler.h
     ../../include/compiler/Options.h
     ../../include/compiler/Program.h
     )
add_library(compiler ${compiler_sources})
target_link_libraries(compiler lexer intermediate codegen optimize splitter)
