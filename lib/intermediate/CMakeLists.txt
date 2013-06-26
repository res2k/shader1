file(GLOB intermediate_sources *.h *.cpp ../include/internal/intermediate/*.h 
			       SequenceOp/*.h SequenceOp/*.cpp ../include/internal/intermediate/SequenceOp/*.h)
add_library(intermediate STATIC ${intermediate_sources})
target_link_libraries(intermediate parser ${ICU_ICUIO_LIBRARIES})

if(CXXTEST_FOUND)
  file(GLOB intermediate_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.cpp)
  CXXTEST_ADD_TEST(test_Intermediate test_Intermediate.cpp
		   ${intermediate_test_sources})
  target_link_libraries(test_Intermediate intermediate)
endif()

