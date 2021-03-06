set(base_sources
    DebugMessageHandler.cpp
    dynamic_tls.cpp
    math_error_handler.cpp
    Mutex.cpp
    PrintAssert.cpp
    format/Formatter.cpp
    ${S1_INC}/base/common.h
    ${S1_INC}/base/DebugMessageHandler.h
    ${S1_INC}/base/dynamic_tls.h
    ${S1_INC}/base/Exception.h
    ${S1_INC}/base/Library.h
    ${S1_INC}/base/LibraryObject.h
    ${S1_INC}/base/math_error_handler.h
    ${S1_INC}/base/MultiOptional.h
    ${S1_INC}/base/Object.h
    ${S1_INC}/base/Result.h
    ${S1_INC}/base/ResultCode_internal.h
    ${S1_INC}/base/format/Sink.h
    ${S1_INC}/base/format/Formatter.h
    ${S1_INC}/base/format/Formatter.ipp
    )
set(base_sources_uc_cpp
    uc/Char.cpp
    uc/char_traits.h
    uc/Exception.cpp
    uc/String.cpp
    uc/StringOverflowException.cpp
    uc/Stream.cpp
    uc/UCDDataLookup.h
    uc/UTF16to8Transcoder.cpp
    uc/UTF8to16Transcoder.cpp
    )
set(base_sources_uc_h
    ${S1_INC}/base/uc/boost_convert.h
    ${S1_INC}/base/uc/Char.h
    ${S1_INC}/base/uc/Exception.h
    ${S1_INC}/base/uc/Stream.h
    ${S1_INC}/base/uc/String.h
    ${S1_INC}/base/uc/StringOverflowException.h
    ${S1_INC}/base/uc/UTF16Decoder.h
    ${S1_INC}/base/uc/UTF16Encoder.h
    ${S1_INC}/base/uc/UTF8Decoder.h
    ${S1_INC}/base/uc/UTF16to8Transcoder.h
    ${S1_INC}/base/uc/UTF8to16Transcoder.h
    )
s1_add_library_internal(base
                        TARGET_LINK ${S1_TARGET_LINK}
                        SOURCES ${base_sources} ${base_sources_uc_cpp} ${base_sources_uc_h}
                        LINK s1_headers_internal)
source_group("uc Headers" FILES ${base_sources_uc_h})
source_group("uc Sources" FILES ${base_sources_uc_cpp})
IF(S1LIB_PDB_CXX_FLAGS)
  target_compile_options(base PRIVATE ${S1LIB_PDB_CXX_FLAGS})
ENDIF()

s1_add_boost_test(test_Base
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/t/bit_scan.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/Char.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/dynamic_tls.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/Formatter.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/Formatter_uc_String.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/LibraryLastError.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/MultiOptional.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/String.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/UnicodeStream.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF16Decoder.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF16Encoder.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF8Decoder.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF8Encoder.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF16to8Transcoder.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/UTF8to16Transcoder.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/Barrier.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/boost_check_equal_ch.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/print_uc_String.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/t/print_wstring.h
                  DEPENDS base)
