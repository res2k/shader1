/*
    Shader1
    Copyright (c) 2015-2018 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include <boost/test/unit_test.hpp>

#include "base/uc/UTF8Encoder.h"

BOOST_AUTO_TEST_SUITE(UTF8Encoder)

// Test UTF8Encoder with null char
BOOST_AUTO_TEST_CASE(UTF8Null)
{
  char chars_utf8[8];
  memset (chars_utf8, 0xff, sizeof (chars_utf8));
  char* output = chars_utf8;
  char* outputEnd = chars_utf8 + sizeof(chars_utf8);
  s1::uc::UTF8Encoder encoder;
  s1::uc::UTF8Encoder::EncodeResult result = encoder (0, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erSuccess);
  BOOST_CHECK_EQUAL(output - chars_utf8, 1);
  BOOST_CHECK_EQUAL(chars_utf8[0], 0);
}

// Test UTF8Encoder with ASCII char
BOOST_AUTO_TEST_CASE(UTF8Simple)
{
  char chars_utf8[8];
  memset (chars_utf8, 0xff, sizeof (chars_utf8));
  char* output = chars_utf8;
  char* outputEnd = chars_utf8 + sizeof(chars_utf8);
  s1::uc::UTF8Encoder encoder;
  s1::uc::UTF8Encoder::EncodeResult result = encoder ('X', output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erSuccess);
  BOOST_CHECK_EQUAL(output - chars_utf8, 1);
  BOOST_CHECK_EQUAL(chars_utf8[0], 'X');
}

// Test UTF8Encoder with a character encoded to 2 bytes
BOOST_AUTO_TEST_CASE(UTF8Encode2)
{
  char chars_utf8[8];
  memset (chars_utf8, 0xff, sizeof (chars_utf8));
  char* output = chars_utf8;
  char* outputEnd = chars_utf8 + sizeof(chars_utf8);
  s1::uc::UTF8Encoder encoder;
  s1::uc::UTF8Encoder::EncodeResult result = encoder (0xeb, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erSuccess);
  BOOST_CHECK_EQUAL(output - chars_utf8, 2);
  BOOST_CHECK_EQUAL(chars_utf8[0], (char)0xc3);
  BOOST_CHECK_EQUAL(chars_utf8[1], (char)0xab);
}

// Test UTF8Encoder with a character encoded to 3 bytes
BOOST_AUTO_TEST_CASE(UTF8Encode3)
{
  char chars_utf8[8];
  memset (chars_utf8, 0xff, sizeof (chars_utf8));
  char* output = chars_utf8;
  char* outputEnd = chars_utf8 + sizeof(chars_utf8);
  s1::uc::UTF8Encoder encoder;
  s1::uc::UTF8Encoder::EncodeResult result = encoder (0x1e37, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erSuccess);
  BOOST_CHECK_EQUAL(output - chars_utf8, 3);
  BOOST_CHECK_EQUAL(chars_utf8[0], (char)0xe1);
  BOOST_CHECK_EQUAL(chars_utf8[1], (char)0xb8);
  BOOST_CHECK_EQUAL(chars_utf8[2], (char)0xb7);
}

// Test UTF8Encoder with a character encoded to 3 bytes
BOOST_AUTO_TEST_CASE(UTF8Encode3b)
{
  char chars_utf8[8];
  memset (chars_utf8, 0xff, sizeof (chars_utf8));
  char* output = chars_utf8;
  char* outputEnd = chars_utf8 + sizeof(chars_utf8);
  s1::uc::UTF8Encoder encoder;
  s1::uc::UTF8Encoder::EncodeResult result = encoder (0xfffd, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erSuccess);
  BOOST_CHECK_EQUAL(output - chars_utf8, 3);
  BOOST_CHECK_EQUAL(chars_utf8[0], (char)0xef);
  BOOST_CHECK_EQUAL(chars_utf8[1], (char)0xbf);
  BOOST_CHECK_EQUAL(chars_utf8[2], (char)0xbd);
}

// Test UTF8Encoder with a character encoded to 4 bytes
BOOST_AUTO_TEST_CASE(UTF8Encode4)
{
  char chars_utf8[8];
  memset (chars_utf8, 0xff, sizeof (chars_utf8));
  char* output = chars_utf8;
  char* outputEnd = chars_utf8 + sizeof(chars_utf8);
  s1::uc::UTF8Encoder encoder;
  s1::uc::UTF8Encoder::EncodeResult result = encoder (0x1f600, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erSuccess);
  BOOST_CHECK_EQUAL(output - chars_utf8, 4);
  BOOST_CHECK_EQUAL(chars_utf8[0], (char)0xf0);
  BOOST_CHECK_EQUAL(chars_utf8[1], (char)0x9f);
  BOOST_CHECK_EQUAL(chars_utf8[2], (char)0x98);
  BOOST_CHECK_EQUAL(chars_utf8[3], (char)0x80);
}

// Test UTF8Encoder with malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed1)
{
  char chars_utf8[8];
  memset (chars_utf8, 0xff, sizeof (chars_utf8));
  char* output = chars_utf8;
  char* outputEnd = chars_utf8 + sizeof(chars_utf8);
  s1::uc::UTF8Encoder encoder;
  s1::uc::UTF8Encoder::EncodeResult result = encoder (0xd83d, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erCharacterInvalid);
  BOOST_CHECK_EQUAL(output - chars_utf8, 0);
}

// Test UTF8Encoder with malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed1b)
{
  char chars_utf8[8];
  memset (chars_utf8, 0xff, sizeof (chars_utf8));
  char* output = chars_utf8;
  char* outputEnd = chars_utf8 + sizeof(chars_utf8);
  s1::uc::UTF8Encoder encoder;
  s1::uc::UTF8Encoder::EncodeResult result = encoder (0xde00, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erCharacterInvalid);
  BOOST_CHECK_EQUAL(output - chars_utf8, 0);
}

// Test UTF8Encoder with malformed input (out of range)
BOOST_AUTO_TEST_CASE(UTF8Malformed2)
{
  char chars_utf8[8];
  memset (chars_utf8, 0xff, sizeof (chars_utf8));
  char* output = chars_utf8;
  char* outputEnd = chars_utf8 + sizeof(chars_utf8);
  s1::uc::UTF8Encoder encoder;
  s1::uc::UTF8Encoder::EncodeResult result = encoder (0x300000, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erCharacterUnsupported);
  BOOST_CHECK_EQUAL(output - chars_utf8, 0);
}

// Test UTF8Encoder buffer underrun
BOOST_AUTO_TEST_CASE(UTF8Underrun)
{
  char chars_utf8[1];
  memset (chars_utf8, 0xff, sizeof (chars_utf8));
  char* output = chars_utf8;
  char* outputEnd = chars_utf8 + sizeof(chars_utf8);
  s1::uc::UTF8Encoder encoder;
  s1::uc::UTF8Encoder::EncodeResult result = encoder (0xeb, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erOutputOverflow);
  BOOST_CHECK_EQUAL(output - chars_utf8, 1);
  BOOST_CHECK_EQUAL(chars_utf8[0], (char)0xc3);
  output = chars_utf8;
  result = encoder (0, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erSuccess);
  BOOST_CHECK_EQUAL(chars_utf8[0], (char)0xab);
  // Make sure encoder resets correctly
  output = chars_utf8;
  result = encoder ('x', output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Encoder::erSuccess);
  BOOST_CHECK_EQUAL(chars_utf8[0], 'x');
}

BOOST_AUTO_TEST_SUITE_END()
