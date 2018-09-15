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

#include "base/uc/UTF16Encoder.h"

BOOST_AUTO_TEST_SUITE(UTF16Encoder)

// Test UTF16Encoder with null char
BOOST_AUTO_TEST_CASE(UTF16Null)
{
  s1::uc::Char16 chars_utf16[2];
  memset (chars_utf16, 0xff, sizeof (chars_utf16));
  s1::uc::Char16* output = chars_utf16;
  s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Encoder encoder;
  s1::uc::UTF16Encoder::EncodeResult result = encoder (0, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Encoder::erSuccess);
  BOOST_CHECK_EQUAL(output - chars_utf16, 1);
  BOOST_CHECK_EQUAL(chars_utf16[0], 0);
}

// Test UTF16Encoder with BMP input
BOOST_AUTO_TEST_CASE(UTF16Simple)
{
  s1::uc::Char16 chars_utf16[2];
  s1::uc::Char16* output = chars_utf16;
  s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Encoder encoder;
  s1::uc::UTF16Encoder::EncodeResult result = encoder (0x20ac, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Encoder::erSuccess);
  BOOST_CHECK_EQUAL(output - chars_utf16, 1);
  BOOST_CHECK_EQUAL(chars_utf16[0], 0x20ac);
}

// Test UTF16Encoder with non-BMP input
BOOST_AUTO_TEST_CASE(UTF16Encoded2)
{
  s1::uc::Char16 chars_utf16[2];
  s1::uc::Char16* output = chars_utf16;
  s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Encoder encoder;
  s1::uc::UTF16Encoder::EncodeResult result = encoder (0x1f600, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Encoder::erSuccess);
  BOOST_CHECK_EQUAL(output - chars_utf16, 2);
  BOOST_CHECK_EQUAL(chars_utf16[0], 0xd83d);
  BOOST_CHECK_EQUAL(chars_utf16[1], 0xde00);
}

// Test UTF16Encoder with malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF16Malformed1)
{
  s1::uc::Char16 chars_utf16[2];
  s1::uc::Char16* output = chars_utf16;
  s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Encoder encoder;
  s1::uc::UTF16Encoder::EncodeResult result = encoder (0xd83d, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Encoder::erCharacterInvalid);
  BOOST_CHECK_EQUAL(output - chars_utf16, 0);
}

// Test UTF16Encoder with malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF16Malformed1b)
{
  s1::uc::Char16 chars_utf16[2];
  s1::uc::Char16* output = chars_utf16;
  s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Encoder encoder;
  s1::uc::UTF16Encoder::EncodeResult result = encoder (0xde00, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Encoder::erCharacterInvalid);
  BOOST_CHECK_EQUAL(output - chars_utf16, 0);
}

// Test UTF16Encoder with malformed input (out of range)
BOOST_AUTO_TEST_CASE(UTF16Malformed2)
{
  s1::uc::Char16 chars_utf16[2];
  s1::uc::Char16* output = chars_utf16;
  s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Encoder encoder;
  s1::uc::UTF16Encoder::EncodeResult result = encoder (0x300000, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Encoder::erCharacterUnsupported);
  BOOST_CHECK_EQUAL(output - chars_utf16, 0);
}

// Test UTF16Encoder buffer underrun
BOOST_AUTO_TEST_CASE(UTF16Underrun)
{
  s1::uc::Char16 chars_utf16[1];
  s1::uc::Char16* output = chars_utf16;
  s1::uc::Char16* outputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Encoder encoder;
  s1::uc::UTF16Encoder::EncodeResult result = encoder (0x1f600, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Encoder::erOutputOverflow);
  BOOST_CHECK_EQUAL(output - chars_utf16, 1);
  BOOST_CHECK_EQUAL(chars_utf16[0], 0xd83d);
  output = chars_utf16;
  result = encoder (0, output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Encoder::erSuccess);
  BOOST_CHECK_EQUAL(chars_utf16[0], 0xde00);
  // Make sure encoder resets correctly
  output = chars_utf16;
  result = encoder ('x', output, outputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Encoder::erSuccess);
  BOOST_CHECK_EQUAL(chars_utf16[0], 'x');
}

BOOST_AUTO_TEST_SUITE_END()
