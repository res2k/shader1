set(base_sources
    UnicodeStream.cpp
    ../../include/base/common.h
    ../../include/base/UnicodeStream.h
    ../../include/base/UnicodeStreamEndOfInputException.h
    ../../include/base/UnicodeStreamException.h
    ../../include/base/UnicodeStreamInvalidCharacterException.h
    )
add_library(base ${base_sources})
target_link_libraries(base ${ICU_LIBRARIES})

if(CXXTEST_FOUND)
  CXXTEST_ADD_TEST(test_UnicodeStream test_UnicodeStream.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/UnicodeStream.cpp)
  target_link_libraries(test_UnicodeStream base)
endif()
