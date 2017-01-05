set(base_sources
    DebugMessageHandler.cpp
    math_error_handler.cpp
    PrintAssert.cpp
    format/Formatter.cpp
    ${S1_INC}/base/common.h
    ${S1_INC}/base/DebugMessageHandler.h
    ${S1_INC}/base/Exception.h
    ${S1_INC}/base/Library.h
    ${S1_INC}/base/LibraryObject.h
    ${S1_INC}/base/math_error_handler.h
    ${S1_INC}/base/Object.h
    ${S1_INC}/base/Result.h
    ${S1_INC}/base/ResultCode_internal.h
    ${S1_INC}/base/format/Sink.h
    ${S1_INC}/base/format/Formatter.h
    ${S1_INC}/base/format/Formatter.tpp
    )
set(base_sources_uc_cpp
    uc/Char.cpp
    uc/char_traits.h
    uc/Exception.cpp
    uc/String.cpp
    uc/Stream.cpp
    uc/StreamException.cpp
    uc/UTF16to8Transcoder.cpp
    uc/UTF8to16Transcoder.cpp
    )
set(base_sources_uc_h
    ${S1_INC}/base/uc/boost_convert.h
    ${S1_INC}/base/uc/Char.h
    ${S1_INC}/base/uc/Exception.h
    ${S1_INC}/base/uc/Stream.h
    ${S1_INC}/base/uc/StreamEndOfInputException.h
    ${S1_INC}/base/uc/StreamException.h
    ${S1_INC}/base/uc/StreamInvalidCharacterException.h
    ${S1_INC}/base/uc/String.h
    ${S1_INC}/base/uc/UTF16Decoder.h
    ${S1_INC}/base/uc/UTF16Encoder.h
    ${S1_INC}/base/uc/UTF8Decoder.h
    ${S1_INC}/base/uc/UTF16to8Transcoder.h
    ${S1_INC}/base/uc/UTF8to16Transcoder.h
    )
s1_add_library(base
               SOURCES ${base_sources} ${base_sources_uc_cpp} ${base_sources_uc_h}
               DEPENDS generate_Object generate_ResultCode)
source_group("uc Headers" FILES ${base_sources_uc_h})
source_group("uc Sources" FILES ${base_sources_uc_cpp})
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
