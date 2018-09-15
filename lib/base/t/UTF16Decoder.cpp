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

#include "base/uc/UTF16Decoder.h"

#include "boost_check_equal_ch.h"

BOOST_AUTO_TEST_SUITE(UTF16Decoder)

// Test UTF16Encoder with BMP input
BOOST_AUTO_TEST_CASE(UTF16Simple)
{
  static const s1::uc::Char16 chars_utf16[] = { 0x20ac };
  const s1::uc::Char16* input = chars_utf16;
  const s1::uc::Char16* inputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Decoder decoder;
  s1::uc::Char32 outChar;
  s1::uc::UTF16Decoder::DecodeResult result = decoder (input, inputEnd, outChar);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Decoder::drSuccess);
  BOOST_CHECK_EQUAL(input - chars_utf16, 1);
  BOOST_CHECK_EQUAL_CH(outChar, 0x20ac);
}

// Test UTF16Encoder with non-BMP input
BOOST_AUTO_TEST_CASE(UTF16Encoded2)
{
  static const s1::uc::Char16 chars_utf16[] = { 0xd83d, 0xde00 };
  const s1::uc::Char16* input = chars_utf16;
  const s1::uc::Char16* inputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Decoder decoder;
  s1::uc::Char32 outChar;
  s1::uc::UTF16Decoder::DecodeResult result = decoder (input, inputEnd, outChar);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Decoder::drSuccess);
  BOOST_CHECK_EQUAL(input - chars_utf16, 2);
  BOOST_CHECK_EQUAL_CH(outChar, 0x1f600);
}

// Test UTF16Encoder with malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF16Malformed1)
{
  static const s1::uc::Char16 chars_utf16[] = { 0xd83d };
  const s1::uc::Char16* input = chars_utf16;
  const s1::uc::Char16* inputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Decoder decoder;
  s1::uc::Char32 outChar;
  s1::uc::UTF16Decoder::DecodeResult result = decoder (input, inputEnd, outChar);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Decoder::drInputUnderrun);
  BOOST_CHECK_EQUAL(input - chars_utf16, 1);
}

// Test UTF16Encoder with malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF16Malformed1b)
{
  static const s1::uc::Char16 chars_utf16[] = { 0xde00 };
  const s1::uc::Char16* input = chars_utf16;
  const s1::uc::Char16* inputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Decoder decoder;
  s1::uc::Char32 outChar;
  s1::uc::UTF16Decoder::DecodeResult result = decoder (input, inputEnd, outChar);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Decoder::drCharacterIncomplete);
  BOOST_CHECK_EQUAL(input - chars_utf16, 1);
  BOOST_CHECK_EQUAL(outChar, s1::uc::ReplacementChar);
}

// Test UTF16Encoder with malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF16Malformed1c)
{
  static const s1::uc::Char16 chars_utf16[] = { 0xd83d, 'x' };
  const s1::uc::Char16* input = chars_utf16;
  const s1::uc::Char16* inputEnd = chars_utf16 + sizeof(chars_utf16)/sizeof(chars_utf16[0]);
  s1::uc::UTF16Decoder decoder;
  s1::uc::Char32 outChar;
  s1::uc::UTF16Decoder::DecodeResult result = decoder (input, inputEnd, outChar);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF16Decoder::drCharacterIncomplete);
  BOOST_CHECK_EQUAL(input - chars_utf16, 1);
  BOOST_CHECK_EQUAL(outChar, s1::uc::ReplacementChar);
}

BOOST_AUTO_TEST_SUITE_END()
