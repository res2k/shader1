file(GLOB intermediate_sources *.h *.cpp ../include/internal/intermediate/*.h)
file(GLOB generator_sources generator/*.h generator/*.cpp)
file(GLOB seqop_sources SequenceOp/*.h SequenceOp/*.cpp ../include/internal/intermediate/SequenceOp/*.h)
include_directories(generator .)
add_library(intermediate_generator OBJECT ${generator_sources})
add_library(intermediate_seqop OBJECT ${seqop_sources})
add_library(intermediate STATIC
			 ${intermediate_sources}
			 $<TARGET_OBJECTS:intermediate_generator>
                         $<TARGET_OBJECTS:intermediate_seqop>)
target_link_libraries(intermediate parser ${ICU_ICUIO_LIBRARIES})

if(CXXTEST_FOUND)
  file(GLOB intermediate_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/t/*.h)
  CXXTEST_ADD_TEST(test_Intermediate test_Intermediate.cpp
		   ${intermediate_test_sources})
  target_link_libraries(test_Intermediate intermediate)
  set_property(TARGET test_Intermediate PROPERTY EXCLUDE_FROM_ALL YES)
  AutoBuildTest(test_Intermediate)
endif()

