file(GLOB compiler_sources *.h *.cpp ../include/compiler/*.h)
add_library(compiler ${compiler_sources})
target_link_libraries(compiler lexer intermediate codegen optimize splitter)
