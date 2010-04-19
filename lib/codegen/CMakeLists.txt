file(GLOB codegen_sources *.h *.cpp ../include/codegen/*.h)
add_library(codegen ${codegen_sources})
target_link_libraries(codegen )

if(CXXTEST_FOUND)
  file(GLOB codegen_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.cpp)
  #CXXTEST_ADD_TEST(test_Codegen test_Codegen.cpp
#		   ${codegen_test_sources})
  #target_link_libraries(test_Codegen codegen)
endif()

