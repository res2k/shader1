set(base_sources
    uc/Char.cpp
    uc/Exception.cpp
    uc/String.cpp
    uc/Stream.cpp
    uc/StreamException.cpp
    uc/UTF8to16Transcoder.cpp
    ../../include/internal/base/common.h
    ../../include/internal/base/Library.h
    ../../include/internal/base/LibraryObject.h
    ../../include/internal/base/Object.h
    ../../include/internal/base/uc/Char.h
    ../../include/internal/base/uc/Exception.h
    ../../include/internal/base/uc/Stream.h
    ../../include/internal/base/uc/StreamEndOfInputException.h
    ../../include/internal/base/uc/StreamException.h
    ../../include/internal/base/uc/StreamInvalidCharacterException.h
    ../../include/internal/base/uc/String.h
    ../../include/internal/base/uc/UTF16Encoder.h
    ../../include/internal/base/uc/UTF8Decoder.h
    ../../include/internal/base/uc/UTF8to16Transcoder.h
    ../../include/internal/base/format/Formatter.h
    ../../include/internal/base/format/Formatter.txx
    )
add_library(base STATIC ${base_sources})
add_dependencies(base generate_Object)
target_link_libraries(base ${ICU_LIBRARIES})

if(CXXTEST_FOUND)
  CXXTEST_ADD_TEST(test_Base test_Base.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/Char.h
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/Formatter.h
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/Formatter_uc_String.h
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/String.h
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/UnicodeStream.h
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF16Decoder.h
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF16Encoder.h
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF8Decoder.h
                   ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF8to16Transcoder.h)
  target_link_libraries(test_Base base)
  set_property(TARGET test_Base PROPERTY EXCLUDE_FROM_ALL YES)
  AutoBuildTest(test_Base)
endif()
