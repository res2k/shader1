file(GLOB splitter_sources *.h *.cpp ../include/internal/splitter/*.h)
s1_add_library(splitter
               SOURCES ${splitter_sources}
               LINK intermediate)
IF(S1LIB_PDB)
  target_compile_options(splitter PRIVATE ${S1LIB_PDB})
ENDIF()

if(CXXTEST_FOUND)
  file(GLOB splitter_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.h)
  s1_add_cxxtest(test_Splitter
		 SOURCES ${splitter_test_sources}
                 DEPENDS intermediate splitter)
endif()

