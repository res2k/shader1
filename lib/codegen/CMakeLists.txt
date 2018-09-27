add_subdirectory(common)
add_subdirectory(sl)
add_subdirectory(glsl)

s1_add_boost_test(test_Codegen
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/t/BasicSequence.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/NameToGLSLIdentifier.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/NestedBlock.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/StringSubstitute.h
                  DEPENDS codegen_glsl codegen_common)
set_property(TARGET test_Codegen
             APPEND
             PROPERTY INCLUDE_DIRECTORIES
             ${CMAKE_CURRENT_SOURCE_DIR}/../intermediate
             ${CMAKE_CURRENT_SOURCE_DIR}/../intermediate/generator)
