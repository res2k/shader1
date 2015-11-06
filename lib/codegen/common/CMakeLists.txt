set(codegen_common_sources
        AnnotatingSequenceCodeGenerator.cpp
        BasicSequenceCodeGenerator.cpp
	StringsArray.cpp
        ../../../include/internal/codegen/common/AnnotatingSequenceCodeGenerator.h
        ../../../include/internal/codegen/common/BasicSequenceCodeGenerator.h
        ../../../include/internal/codegen/common/Options.h
        ../../../include/internal/codegen/common/OptionsBase.h
	../../../include/internal/codegen/common/StringsArray.h)
s1_add_library(codegen_common
               SOURCES ${codegen_common_sources}
               LINK intermediate)
IF(S1LIB_PDB)
  target_compile_options(codegen_common PRIVATE ${S1LIB_PDB})
ENDIF()
