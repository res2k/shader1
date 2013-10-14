file(GLOB optimize_sources *.h *.cpp ../include/internal/optimize/*.h)
add_library(optimize STATIC ${optimize_sources})
target_link_libraries(optimize intermediate)

if(CXXTEST_FOUND)
  file(GLOB optimize_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.cpp)
  CXXTEST_ADD_TEST(test_Optimize test_Optimize.cpp
		   ${optimize_test_sources})
  target_link_libraries(test_Optimize optimize)
  set_property(TARGET test_Optimize PROPERTY EXCLUDE_FROM_ALL YES)
  AutoBuildTest(test_Optimize)
endif()
