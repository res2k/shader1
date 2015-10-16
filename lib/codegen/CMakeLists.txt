file(GLOB codegen_cg_sources cg/*.h cg/*.cpp)
s1_add_library(codegen_cg SOURCES ${codegen_cg_sources})
IF(S1LIB_PDB)
  target_compile_options(codegen_cg PRIVATE ${S1LIB_PDB})
ENDIF()
set(codegen_sources
	CgGenerator.cpp
        common/AnnotatingSequenceCodeGenerator.cpp
        common/BasicSequenceCodeGenerator.cpp
	common/StringsArray.cpp
	../../include/internal/codegen/CgGenerator.h
        ../../include/internal/codegen/common/AnnotatingSequenceCodeGenerator.h
        ../../include/internal/codegen/common/BasicSequenceCodeGenerator.h
	../../include/internal/codegen/common/StringsArray.h)
s1_add_library(codegen
               SOURCES ${codegen_sources}
               LINK codegen_cg intermediate)
IF(S1LIB_PDB)
  target_compile_options(codegen PRIVATE ${S1LIB_PDB})
ENDIF()


if(CXXTEST_FOUND)
  file(GLOB codegen_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.h)
  s1_add_cxxtest(test_Codegen
		 SOURCES ${codegen_test_sources}
                 DEPENDS codegen)
  set_property(TARGET test_Codegen
               APPEND
               PROPERTY INCLUDE_DIRECTORIES
               ${CMAKE_CURRENT_SOURCE_DIR}/../intermediate
               ${CMAKE_CURRENT_SOURCE_DIR}/../intermediate/generator)
endif()

