file(GLOB seqop_sources *.h *.cpp ${CMAKE_SOURCE_DIR}/include/internal/intermediate/SequenceOp/*.h)
s1_add_library(intermediate_seqop SOURCES ${seqop_sources})
