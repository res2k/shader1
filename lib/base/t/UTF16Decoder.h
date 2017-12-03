/*
    Shader1
    Copyright (c) 2015 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <cxxtest/TestSuite.h>

#include "base/uc/UTF16Decoder.h"

#include "assert_equals_ch.h"

class UTF16DecoderSuite : public CxxTest::TestSuite 
{
public:
  // Test UTF16Encoder with BMP input
  void testUTF16Simple ()
  {
    static const s1::uc::Char16 chars_utf16[] = { 0x20ac };
    const s1::uc::Char16* input = chars_utf16;
    const s1::uc::Char16* inputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Decoder decoder;
    s1::uc::Char32 outChar;
    s1::uc::UTF16Decoder::DecodeResult result = decoder (input, inputEnd, outChar);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Decoder::drSuccess);
    TS_ASSERT_EQUALS(input - chars_utf16, 1);
    TS_ASSERT_EQUALS_CH(outChar, 0x20ac);
  }

  // Test UTF16Encoder with non-BMP input
  void testUTF16Encoded2 ()
  {
    static const s1::uc::Char16 chars_utf16[] = { 0xd83d, 0xde00 };
    const s1::uc::Char16* input = chars_utf16;
    const s1::uc::Char16* inputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Decoder decoder;
    s1::uc::Char32 outChar;
    s1::uc::UTF16Decoder::DecodeResult result = decoder (input, inputEnd, outChar);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Decoder::drSuccess);
    TS_ASSERT_EQUALS(input - chars_utf16, 2);
    TS_ASSERT_EQUALS_CH(outChar, 0x1f600);
  }

  // Test UTF16Encoder with malformed input (surrogate half)
  void testUTF16Malformed1 ()
  {
    static const s1::uc::Char16 chars_utf16[] = { 0xd83d };
    const s1::uc::Char16* input = chars_utf16;
    const s1::uc::Char16* inputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Decoder decoder;
    s1::uc::Char32 outChar;
    s1::uc::UTF16Decoder::DecodeResult result = decoder (input, inputEnd, outChar);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Decoder::drInputUnderrun);
    TS_ASSERT_EQUALS(input - chars_utf16, 1);
  }

  // Test UTF16Encoder with malformed input (surrogate half)
  void testUTF16Malformed1b ()
  {
    static const s1::uc::Char16 chars_utf16[] = { 0xde00 };
    const s1::uc::Char16* input = chars_utf16;
    const s1::uc::Char16* inputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Decoder decoder;
    s1::uc::Char32 outChar;
    s1::uc::UTF16Decoder::DecodeResult result = decoder (input, inputEnd, outChar);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Decoder::drCharacterIncomplete);
    TS_ASSERT_EQUALS(input - chars_utf16, 1);
    TS_ASSERT_EQUALS(outChar, s1::uc::ReplacementChar);
  }

  // Test UTF16Encoder with malformed input (surrogate half)
  void testUTF16Malformed1c ()
  {
    static const s1::uc::Char16 chars_utf16[] = { 0xd83d, 'x' };
    const s1::uc::Char16* input = chars_utf16;
    const s1::uc::Char16* inputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Decoder decoder;
    s1::uc::Char32 outChar;
    s1::uc::UTF16Decoder::DecodeResult result = decoder (input, inputEnd, outChar);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Decoder::drCharacterIncomplete);
    TS_ASSERT_EQUALS(input - chars_utf16, 1);
    TS_ASSERT_EQUALS(outChar, s1::uc::ReplacementChar);
  }
};
