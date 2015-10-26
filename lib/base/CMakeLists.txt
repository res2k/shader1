set(base_sources
    format/Formatter.cpp
    uc/Char.cpp
    uc/char_traits.h
    uc/Exception.cpp
    uc/String.cpp
    uc/Stream.cpp
    uc/StreamException.cpp
    uc/UTF16to8Transcoder.cpp
    uc/UTF8to16Transcoder.cpp
    ../../include/internal/base/common.h
    ../../include/internal/base/Library.h
    ../../include/internal/base/LibraryObject.h
    ../../include/internal/base/Object.h
    ../../include/internal/base/Result.h
    ../../include/internal/base/uc/Char.h
    ../../include/internal/base/uc/Exception.h
    ../../include/internal/base/uc/Stream.h
    ../../include/internal/base/uc/StreamEndOfInputException.h
    ../../include/internal/base/uc/StreamException.h
    ../../include/internal/base/uc/StreamInvalidCharacterException.h
    ../../include/internal/base/uc/String.h
    ../../include/internal/base/uc/UTF16Decoder.h
    ../../include/internal/base/uc/UTF16Encoder.h
    ../../include/internal/base/uc/UTF8Decoder.h
    ../../include/internal/base/uc/UTF16to8Transcoder.h
    ../../include/internal/base/uc/UTF8to16Transcoder.h
    ../../include/internal/base/format/Formatter.h
    ../../include/internal/base/format/Formatter.tpp
    )
s1_get_boost_link_libs(_BASE_LINK_BOOST thread)
s1_add_library(base
               SOURCES ${base_sources}
               DEPENDS generate_Object
               LINK ${_BASE_LINK_BOOST})
IF(S1LIB_PDB)
  target_compile_options(base PRIVATE ${S1LIB_PDB})
ENDIF()

if(CXXTEST_FOUND)
  s1_add_cxxtest(test_Base
                 SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/t/Char.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/t/Formatter.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/t/Formatter_uc_String.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/t/String.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/t/UnicodeStream.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF16Decoder.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF16Encoder.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF8Decoder.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF8Encoder.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF16to8Transcoder.h
                         ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF8to16Transcoder.h
                 DEPENDS base)
endif()
