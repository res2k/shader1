file(GLOB codegen_cg_sources cg/*.h cg/*.cpp)
add_library(codegen_cg ${codegen_cg_sources})
set(codegen_sources
	CgGenerator.cpp
	StringsArray.cpp
	../../include/internal/codegen/CgGenerator.h
	../../include/internal/codegen/StringsArray.h)
add_library(codegen ${codegen_sources})
target_link_libraries(codegen codegen_cg intermediate)

if(CXXTEST_FOUND)
  file(GLOB codegen_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.cpp)
  CXXTEST_ADD_TEST(test_Codegen test_Codegen.cpp
		   ${codegen_test_sources})
  target_link_libraries(test_Codegen codegen)
endif()

