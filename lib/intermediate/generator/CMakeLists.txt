file(GLOB generator_sources *.h *.cpp)
s1_add_library(intermediate_generator SOURCES ${generator_sources})
