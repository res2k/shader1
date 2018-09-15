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

#include <boost/test/unit_test.hpp>

#include "base/uc/UTF8Decoder.h"

#include "boost_check_equal_ch.h"

BOOST_AUTO_TEST_SUITE(UTF8Decoder)

BOOST_AUTO_TEST_CASE(Empty)
{
  const char nullStr[] = "";
  s1::uc::UTF8Decoder decoder;
  const char* input = nullStr;
  const char* inputEnd = nullStr + sizeof(nullStr) - 1;
  s1::uc::Char32 ch (0x12345678);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drInputUnderrun);
  BOOST_CHECK_EQUAL_CH(ch, 0x12345678);
}

// Test UTF8Decoder with ASCII input
BOOST_AUTO_TEST_CASE(UTF8Simple)
{
  const char chars_utf8[] = "X";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drSuccess);
  BOOST_CHECK_EQUAL_CH(ch, 'X');
}

// Test UTF8Decoder with a character encoded in 2 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded2)
{
  const char chars_utf8[] = "\xc3\xab";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drSuccess);
  BOOST_CHECK_EQUAL_CH(ch, 0xeb);
}

// Test UTF8Decoder with characters encoded in 3 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded3)
{
  const char chars_utf8[] = "\xe1\xb8\xb7";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drSuccess);
  BOOST_CHECK_EQUAL_CH(ch, 0x1e37);
}

// Test UTF8Decoder with characters encoded in 4 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded4)
{
  const char chars_utf8[] = "\xf0\x9f\x98\x80";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drSuccess);
  BOOST_CHECK_EQUAL_CH(ch, 0x1f600);
}

// Test UTF8Decoder with malformed input (overlong encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed1)
{
  const char chars_utf8[] = "\xc0\x8a";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drEncodingInvalid);
  BOOST_CHECK_EQUAL_CH(ch, 0xfffd);
}

// Test UTF8Decoder with malformed input (broken encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed2)
{
  const char chars_utf8[] = "\xc3";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0x12345678);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drInputUnderrun);
  BOOST_CHECK_EQUAL_CH(ch, 0x12345678);
}

// Test UTF8Decoder with malformed input (broken encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed2b)
{
  const char chars_utf8[] = "\xab";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drCharacterIncomplete);
  BOOST_CHECK_EQUAL_CH(ch, 0xfffd);
}

// Test UTF8Decoder with malformed input (encoded surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed3)
{
  const char chars_utf8[] = "\xed\xa0\x80";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drCharacterInvalid);
  BOOST_CHECK_EQUAL_CH(ch, 0xfffd);
}

// Test UTF8Decoder with malformed input (encoded surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed4)
{
  const char chars_utf8[] = "\xed\xb0\x80";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drCharacterInvalid);
  BOOST_CHECK_EQUAL_CH(ch, 0xfffd);
}

// Test UTF8Decoder with malformed input (overlong encoding) and continuation afterwards
BOOST_AUTO_TEST_CASE(UTF8Malformed1cont)
{
  const char chars_utf8[] = "\xc0\x8a*";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd - 1);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drEncodingInvalid);
  BOOST_CHECK_EQUAL_CH(ch, 0xfffd);
  result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drSuccess);
  BOOST_CHECK_EQUAL_CH(ch, '*');
}

// Test UTF8Decoder with malformed input (broken encoding) and continuation afterwards
BOOST_AUTO_TEST_CASE(UTF8Malformed2cont)
{
  const char chars_utf8[] = "\xc3*";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd - 1);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drCharacterIncomplete);
  BOOST_CHECK_EQUAL_CH(ch, 0xfffd);
  result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drSuccess);
  BOOST_CHECK_EQUAL_CH(ch, '*');
}

// Test UTF8Decoder with malformed input (broken encoding) and continuation afterwards
BOOST_AUTO_TEST_CASE(UTF8Malformed2bcont)
{
  const char chars_utf8[] = "\xab*";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd - 1);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drCharacterIncomplete);
  BOOST_CHECK_EQUAL_CH(ch, 0xfffd);
  result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drSuccess);
  BOOST_CHECK_EQUAL_CH(ch, '*');
}

// Test UTF8Decoder with malformed input (broken encoding) and continuation afterwards
BOOST_AUTO_TEST_CASE(UTF8Malformed2ccont)
{
  const char chars_utf8[] = "\xe1\xb8\xc3\xab";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd - 2);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drCharacterIncomplete);
  BOOST_CHECK_EQUAL_CH(ch, 0xfffd);
  result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drSuccess);
  BOOST_CHECK_EQUAL_CH(ch, 0xeb);
}

// Test UTF8Decoder with malformed input (broken encoding) and continuation afterwards
BOOST_AUTO_TEST_CASE(UTF8Malformed2dcont)
{
  const char chars_utf8[] = "\xab\xe1\xb8\xb7";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd - 3);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drCharacterIncomplete);
  BOOST_CHECK_EQUAL_CH(ch, 0xfffd);
  result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drSuccess);
  BOOST_CHECK_EQUAL_CH(ch, 0x1e37);
}

// Test UTF8Decoder with malformed input (encoded surrogate half) and continuation afterwards
BOOST_AUTO_TEST_CASE(UTF8Malformed3cont)
{
  const char chars_utf8[] = "\xed\xa0\x80*";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd - 1);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drCharacterInvalid);
  BOOST_CHECK_EQUAL_CH(ch, 0xfffd);
  result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drSuccess);
  BOOST_CHECK_EQUAL_CH(ch, '*');
}

// Test UTF8Decoder with malformed input (encoded surrogate half) and continuation afterwards
BOOST_AUTO_TEST_CASE(UTF8Malformed4cont)
{
  const char chars_utf8[] = "\xed\xb0\x80*";
  s1::uc::UTF8Decoder decoder;
  const char* input = chars_utf8;
  const char* inputEnd = chars_utf8 + sizeof(chars_utf8) - 1;
  s1::uc::Char32 ch (0);
  s1::uc::UTF8Decoder::DecodeResult result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd - 1);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drCharacterInvalid);
  BOOST_CHECK_EQUAL_CH(ch, 0xfffd);
  result = decoder (input, inputEnd, ch);
  BOOST_CHECK_EQUAL(input, inputEnd);
  BOOST_CHECK_EQUAL(result, s1::uc::UTF8Decoder::drSuccess);
  BOOST_CHECK_EQUAL_CH(ch, '*');
}

BOOST_AUTO_TEST_SUITE_END()
