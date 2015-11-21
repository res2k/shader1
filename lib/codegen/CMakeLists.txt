add_subdirectory(common)
add_subdirectory(sl)
add_subdirectory(cg)
add_subdirectory(glsl)

if(CXXTEST_FOUND)
  file(GLOB codegen_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.h)
  s1_add_cxxtest(test_Codegen
		 SOURCES ${codegen_test_sources}
                 DEPENDS codegen_cg codegen_common)
  set_property(TARGET test_Codegen
               APPEND
               PROPERTY INCLUDE_DIRECTORIES
               ${CMAKE_CURRENT_SOURCE_DIR}/../intermediate
               ${CMAKE_CURRENT_SOURCE_DIR}/../intermediate/generator)
endif()

