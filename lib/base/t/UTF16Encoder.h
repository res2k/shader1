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

#include "base/uc/UTF16Encoder.h"

class UTF16EncoderSuite : public CxxTest::TestSuite 
{
public:
  // Test UTF16Encoder with null char
  void testUTF16Null ()
  {
    s1::uc::Char16 chars_utf16[2];
    memset (chars_utf16, 0xff, sizeof (chars_utf16));
    s1::uc::Char16* output = chars_utf16;
    s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Encoder encoder;
    s1::uc::UTF16Encoder::EncodeResult result = encoder (0, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Encoder::erSuccess);
    TS_ASSERT_EQUALS(output - chars_utf16, 1);
    TS_ASSERT_EQUALS(chars_utf16[0], 0);
  }

  // Test UTF16Encoder with BMP input
  void testUTF16Simple ()
  {
    s1::uc::Char16 chars_utf16[2];
    s1::uc::Char16* output = chars_utf16;
    s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Encoder encoder;
    s1::uc::UTF16Encoder::EncodeResult result = encoder (0x20ac, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Encoder::erSuccess);
    TS_ASSERT_EQUALS(output - chars_utf16, 1);
    TS_ASSERT_EQUALS(chars_utf16[0], 0x20ac);
  }

  // Test UTF16Encoder with non-BMP input
  void testUTF16Encoded2 ()
  {
    s1::uc::Char16 chars_utf16[2];
    s1::uc::Char16* output = chars_utf16;
    s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Encoder encoder;
    s1::uc::UTF16Encoder::EncodeResult result = encoder (0x1f600, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Encoder::erSuccess);
    TS_ASSERT_EQUALS(output - chars_utf16, 2);
    TS_ASSERT_EQUALS(chars_utf16[0], 0xd83d);
    TS_ASSERT_EQUALS(chars_utf16[1], 0xde00);
  }

  // Test UTF16Encoder with malformed input (surrogate half)
  void testUTF16Malformed1 ()
  {
    s1::uc::Char16 chars_utf16[2];
    s1::uc::Char16* output = chars_utf16;
    s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Encoder encoder;
    s1::uc::UTF16Encoder::EncodeResult result = encoder (0xd83d, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Encoder::erCharacterInvalid);
    TS_ASSERT_EQUALS(output - chars_utf16, 0);
  }

  // Test UTF16Encoder with malformed input (surrogate half)
  void testUTF16Malformed1b ()
  {
    s1::uc::Char16 chars_utf16[2];
    s1::uc::Char16* output = chars_utf16;
    s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Encoder encoder;
    s1::uc::UTF16Encoder::EncodeResult result = encoder (0xde00, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Encoder::erCharacterInvalid);
    TS_ASSERT_EQUALS(output - chars_utf16, 0);
  }

  // Test UTF16Encoder with malformed input (out of range)
  void testUTF16Malformed2 ()
  {
    s1::uc::Char16 chars_utf16[2];
    s1::uc::Char16* output = chars_utf16;
    s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Encoder encoder;
    s1::uc::UTF16Encoder::EncodeResult result = encoder (0x300000, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Encoder::erCharacterUnsupported);
    TS_ASSERT_EQUALS(output - chars_utf16, 0);
  }

  // Test UTF16Encoder buffer underrun
  void testUTF16Underrun ()
  {
    s1::uc::Char16 chars_utf16[1];
    s1::uc::Char16* output = chars_utf16;
    s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
    s1::uc::UTF16Encoder encoder;
    s1::uc::UTF16Encoder::EncodeResult result = encoder (0x1f600, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Encoder::erOutputOverflow);
    TS_ASSERT_EQUALS(output - chars_utf16, 1);
    TS_ASSERT_EQUALS(chars_utf16[0], 0xd83d);
    output = chars_utf16;
    result = encoder (0, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Encoder::erSuccess);
    TS_ASSERT_EQUALS(chars_utf16[0], 0xde00);
    // Make sure encoder resets correctly
    output = chars_utf16;
    result = encoder ('x', output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF16Encoder::erSuccess);
    TS_ASSERT_EQUALS(chars_utf16[0], 'x');
  }
};
