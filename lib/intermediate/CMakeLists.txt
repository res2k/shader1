file(GLOB intermediate_sources *.h *.cpp ../include/internal/intermediate/*.h)
file(GLOB generator_sources generator/*.h generator/*.cpp)
file(GLOB seqop_sources SequenceOp/*.h SequenceOp/*.cpp ../include/internal/intermediate/SequenceOp/*.h)
include_directories(generator .)
s1_add_library(intermediate_generator SOURCES ${generator_sources})
s1_add_library(intermediate_seqop SOURCES ${seqop_sources})
s1_add_library(intermediate
               SOURCES ${intermediate_sources}
               LINK intermediate_generator intermediate_seqop parser)
IF(S1LIB_PDB)
  target_compile_options(intermediate_generator PRIVATE ${S1LIB_PDB})
  target_compile_options(intermediate_seqop PRIVATE ${S1LIB_PDB})
  target_compile_options(intermediate PRIVATE ${S1LIB_PDB})
ENDIF()

if(CXXTEST_FOUND)
  file(GLOB intermediate_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.h)
  s1_add_cxxtest(test_Intermediate
		 SOURCES ${intermediate_test_sources}
                 DEPENDS intermediate)
  set_property(TARGET test_Intermediate
               APPEND
               PROPERTY INCLUDE_DIRECTORIES
               ${CMAKE_CURRENT_SOURCE_DIR}/generator)
endif()

