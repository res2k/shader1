set(codegen_common_sources
    AnnotatingSequenceCodeGenerator.cpp
    BasicSequenceCodeGenerator.cpp
    StringsArray.cpp
    UniqueIdentifiersSet.cpp
    ${S1_INC}/codegen/common/AnnotatingSequenceCodeGenerator.h
    ${S1_INC}/codegen/common/BasicSequenceCodeGenerator.h
    ${S1_INC}/codegen/common/Options.h
    ${S1_INC}/codegen/common/OptionsBase.h
    ${S1_INC}/codegen/common/StringsArray.h
    ${S1_INC}/codegen/common/UniqueIdentifiersSet.h
    )
s1_add_library_internal(codegen_common
                        TARGET_LINK ${S1_TARGET_LINK}
                        SOURCES ${codegen_common_sources}
                        LINK intermediate)
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(codegen_common PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()
