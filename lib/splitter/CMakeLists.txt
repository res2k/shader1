set(splitter_sources
    ProgramSplitter.cpp
    SequenceSplitter.cpp
    ${CMAKE_SOURCE_DIR}/include/internal/splitter/ProgramSplitter.h
    ${CMAKE_SOURCE_DIR}/include/internal/splitter/SequenceSplitter.h
    )
s1_add_library(splitter
               SOURCES ${splitter_sources}
               LINK intermediate)
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(splitter PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()

s1_add_boost_test(test_Splitter
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/t/NestedBlock.cpp
                  DEPENDS intermediate splitter)
