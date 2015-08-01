file(GLOB optimize_sources *.h *.cpp ../include/internal/optimize/*.h)
s1_add_library(optimize
               SOURCES ${optimize_sources}
               LINK intermediate)
IF(S1LIB_PDB)
  target_compile_options(optimize PRIVATE ${S1LIB_PDB})
ENDIF()

if(CXXTEST_FOUND)
  file(GLOB optimize_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.h)
  s1_add_cxxtest(test_Optimize
		 SOURCES ${optimize_test_sources}
                 DEPENDS optimize)
  set_property(TARGET test_Optimize
               APPEND
               PROPERTY INCLUDE_DIRECTORIES
               ${CMAKE_CURRENT_SOURCE_DIR}/../intermediate
               ${CMAKE_CURRENT_SOURCE_DIR}/../intermediate/generator)
endif()
