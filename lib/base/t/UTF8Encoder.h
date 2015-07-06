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

#include "base/uc/UTF8Encoder.h"

class UTF8EncoderSuite : public CxxTest::TestSuite 
{
public:
  // Test UTF8Encoder with null char
  void testUTF8Null ()
  {
    char chars_utf8[8];
    memset (chars_utf8, 0xff, sizeof (chars_utf8));
    char* output = chars_utf8;
    char* outputEnd = chars_utf8 + sizeof(chars_utf8);
    s1::uc::UTF8Encoder encoder;
    s1::uc::UTF8Encoder::EncodeResult result = encoder (0, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erSuccess);
    TS_ASSERT_EQUALS(output - chars_utf8, 1);
    TS_ASSERT_EQUALS(chars_utf8[0], 0);
  }

  // Test UTF8Encoder with ASCII char
  void testUTF8Simple ()
  {
    char chars_utf8[8];
    memset (chars_utf8, 0xff, sizeof (chars_utf8));
    char* output = chars_utf8;
    char* outputEnd = chars_utf8 + sizeof(chars_utf8);
    s1::uc::UTF8Encoder encoder;
    s1::uc::UTF8Encoder::EncodeResult result = encoder ('X', output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erSuccess);
    TS_ASSERT_EQUALS(output - chars_utf8, 1);
    TS_ASSERT_EQUALS(chars_utf8[0], 'X');
  }

  // Test UTF8Encoder with a character encoded to 2 bytes
  void testUTF8Encode2 ()
  {
    char chars_utf8[8];
    memset (chars_utf8, 0xff, sizeof (chars_utf8));
    char* output = chars_utf8;
    char* outputEnd = chars_utf8 + sizeof(chars_utf8);
    s1::uc::UTF8Encoder encoder;
    s1::uc::UTF8Encoder::EncodeResult result = encoder (0xeb, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erSuccess);
    TS_ASSERT_EQUALS(output - chars_utf8, 2);
    TS_ASSERT_EQUALS(chars_utf8[0], (char)0xc3);
    TS_ASSERT_EQUALS(chars_utf8[1], (char)0xab);
  }

  // Test UTF8Encoder with a character encoded to 3 bytes
  void testUTF8Encode3 ()
  {
    char chars_utf8[8];
    memset (chars_utf8, 0xff, sizeof (chars_utf8));
    char* output = chars_utf8;
    char* outputEnd = chars_utf8 + sizeof(chars_utf8);
    s1::uc::UTF8Encoder encoder;
    s1::uc::UTF8Encoder::EncodeResult result = encoder (0x1e37, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erSuccess);
    TS_ASSERT_EQUALS(output - chars_utf8, 3);
    TS_ASSERT_EQUALS(chars_utf8[0], (char)0xe1);
    TS_ASSERT_EQUALS(chars_utf8[1], (char)0xb8);
    TS_ASSERT_EQUALS(chars_utf8[2], (char)0xb7);
  }

  // Test UTF8Encoder with a character encoded to 3 bytes
  void testUTF8Encode3b ()
  {
    char chars_utf8[8];
    memset (chars_utf8, 0xff, sizeof (chars_utf8));
    char* output = chars_utf8;
    char* outputEnd = chars_utf8 + sizeof(chars_utf8);
    s1::uc::UTF8Encoder encoder;
    s1::uc::UTF8Encoder::EncodeResult result = encoder (0xfffd, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erSuccess);
    TS_ASSERT_EQUALS(output - chars_utf8, 3);
    TS_ASSERT_EQUALS(chars_utf8[0], (char)0xef);
    TS_ASSERT_EQUALS(chars_utf8[1], (char)0xbf);
    TS_ASSERT_EQUALS(chars_utf8[2], (char)0xbd);
  }

  // Test UTF8Encoder with a character encoded to 4 bytes
  void testUTF8Encode4 ()
  {
    char chars_utf8[8];
    memset (chars_utf8, 0xff, sizeof (chars_utf8));
    char* output = chars_utf8;
    char* outputEnd = chars_utf8 + sizeof(chars_utf8);
    s1::uc::UTF8Encoder encoder;
    s1::uc::UTF8Encoder::EncodeResult result = encoder (0x1f600, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erSuccess);
    TS_ASSERT_EQUALS(output - chars_utf8, 4);
    TS_ASSERT_EQUALS(chars_utf8[0], (char)0xf0);
    TS_ASSERT_EQUALS(chars_utf8[1], (char)0x9f);
    TS_ASSERT_EQUALS(chars_utf8[2], (char)0x98);
    TS_ASSERT_EQUALS(chars_utf8[3], (char)0x80);
  }

  // Test UTF8Encoder with malformed input (surrogate half)
  void testUTF8Malformed1 ()
  {
    char chars_utf8[8];
    memset (chars_utf8, 0xff, sizeof (chars_utf8));
    char* output = chars_utf8;
    char* outputEnd = chars_utf8 + sizeof(chars_utf8);
    s1::uc::UTF8Encoder encoder;
    s1::uc::UTF8Encoder::EncodeResult result = encoder (0xd83d, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erCharacterInvalid);
    TS_ASSERT_EQUALS(output - chars_utf8, 0);
  }

  // Test UTF8Encoder with malformed input (surrogate half)
  void testUTF8Malformed1b ()
  {
    char chars_utf8[8];
    memset (chars_utf8, 0xff, sizeof (chars_utf8));
    char* output = chars_utf8;
    char* outputEnd = chars_utf8 + sizeof(chars_utf8);
    s1::uc::UTF8Encoder encoder;
    s1::uc::UTF8Encoder::EncodeResult result = encoder (0xde00, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erCharacterInvalid);
    TS_ASSERT_EQUALS(output - chars_utf8, 0);
  }

  // Test UTF8Encoder with malformed input (out of range)
  void testUTF8Malformed2 ()
  {
    char chars_utf8[8];
    memset (chars_utf8, 0xff, sizeof (chars_utf8));
    char* output = chars_utf8;
    char* outputEnd = chars_utf8 + sizeof(chars_utf8);
    s1::uc::UTF8Encoder encoder;
    s1::uc::UTF8Encoder::EncodeResult result = encoder (0x300000, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erCharacterUnsupported);
    TS_ASSERT_EQUALS(output - chars_utf8, 0);
  }

  // Test UTF8Encoder buffer underrun
  void testUTF8Underrun ()
  {
    char chars_utf8[1];
    memset (chars_utf8, 0xff, sizeof (chars_utf8));
    char* output = chars_utf8;
    char* outputEnd = chars_utf8 + sizeof(chars_utf8);
    s1::uc::UTF8Encoder encoder;
    s1::uc::UTF8Encoder::EncodeResult result = encoder (0xeb, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erOutputOverflow);
    TS_ASSERT_EQUALS(output - chars_utf8, 1);
    TS_ASSERT_EQUALS(chars_utf8[0], (char)0xc3);
    output = chars_utf8;
    result = encoder (0, output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erSuccess);
    TS_ASSERT_EQUALS(chars_utf8[0], (char)0xab);
    // Make sure encoder resets correctly
    output = chars_utf8;
    result = encoder ('x', output, outputEnd);
    TS_ASSERT_EQUALS(result, s1::uc::UTF8Encoder::erSuccess);
    TS_ASSERT_EQUALS(chars_utf8[0], 'x');
  }
};
