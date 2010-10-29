file(GLOB splitter_sources *.h *.cpp ../include/splitter/*.h)
add_library(splitter ${splitter_sources})

if(CXXTEST_FOUND)
  file(GLOB splitter_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.cpp)
  CXXTEST_ADD_TEST(test_Splitter test_Splitter.cpp
		   ${splitter_test_sources})
  target_link_libraries(test_Splitter intermediate splitter)
endif()

