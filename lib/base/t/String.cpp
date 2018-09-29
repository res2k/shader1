/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


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

#include <boost/format.hpp>

#include <string>

#include "base/uc/String.h"
#include "base/uc/StringOverflowException.h"
#include "print_uc_String.h"

typedef s1::uc::Char32 Char32;

#include "data/NormalizationTestData.inc"

BOOST_AUTO_TEST_SUITE(String)

static s1::uc::String MakeLongString (s1::uc::String::size_type num = 128)
{
  s1::uc::String s;
  s.reserve (num);
  for (s1::uc::String::size_type n = 0; n < num; n++)
  {
    s.append (static_cast<s1::uc::Char> ('a' + (n % 26)));
  }
  return s;
}
// Uppercase variant
static s1::uc::String MakeLongString2 (s1::uc::String::size_type num = 128)
{
  s1::uc::String s;
  s.reserve (num);
  for (s1::uc::String::size_type n = 0; n < num; n++)
  {
    s.append (static_cast<s1::uc::Char> ('A' + (n % 26)));
  }
  return s;
}

BOOST_AUTO_TEST_CASE(Empty1)
{
  s1::uc::String empty;
  BOOST_CHECK(empty.isEmpty());
  BOOST_CHECK_EQUAL(empty.length(), 0u);
  BOOST_CHECK_EQUAL(empty.size(), 0u);
  BOOST_CHECK_EQUAL(empty.data()[empty.size()], 0);
}

BOOST_AUTO_TEST_CASE(Empty2)
{
  s1::uc::String empty ("");
  BOOST_CHECK(empty.isEmpty());
  BOOST_CHECK_EQUAL(empty.length(), 0u);
  BOOST_CHECK_EQUAL(empty.size(), 0u);
  BOOST_CHECK_EQUAL(empty.data()[empty.size()], 0);
}

BOOST_AUTO_TEST_CASE(Empty3)
{
  const s1::uc::Char nullStr[] = { 0 };
  s1::uc::String empty (nullStr);
  BOOST_CHECK(empty.isEmpty());
  BOOST_CHECK_EQUAL(empty.length(), 0u);
  BOOST_CHECK_EQUAL(empty.size(), 0u);
  BOOST_CHECK_EQUAL(empty.data()[empty.size()], 0);
}

BOOST_AUTO_TEST_CASE(OneChar1)
{
  s1::uc::String str ("a");
  BOOST_CHECK(!str.isEmpty());
  BOOST_CHECK_EQUAL(str.length(), 1u);
  BOOST_CHECK_EQUAL(str.data()[0], 'a');
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

BOOST_AUTO_TEST_CASE(OneChar2)
{
  const s1::uc::Char aStr[] = { 'a', 0 };
  s1::uc::String str (aStr);
  BOOST_CHECK(!str.isEmpty());
  BOOST_CHECK_EQUAL(str.length(), 1u);
  BOOST_CHECK_EQUAL(str.data()[0], 'a');
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

BOOST_AUTO_TEST_CASE(OneChar3)
{
  s1::uc::String str ('a');
  BOOST_CHECK(!str.isEmpty());
  BOOST_CHECK_EQUAL(str.length(), 1u);
  BOOST_CHECK_EQUAL(str.data()[0], 'a');
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

BOOST_AUTO_TEST_CASE(MultiChar1)
{
  s1::uc::String str ("Hello");
  const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(str.length(), 5u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

BOOST_AUTO_TEST_CASE(MultiChar2)
{
  const s1::uc::Char32 chars_utf32[] = { 'H', 'e', 'l', 'l', 'o', 0 };
  s1::uc::String str (chars_utf32);
  const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(str.length(), 5u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

BOOST_AUTO_TEST_CASE(UTF32Single)
{
  s1::uc::String str (0x1f435);
  const s1::uc::Char chars_utf16[] = { 0xD83D, 0xDC35, 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK(!str.isEmpty());
  BOOST_CHECK_EQUAL(str.length(), 2u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.countChar32(), 1u);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

BOOST_AUTO_TEST_CASE(UTF32Multi)
{
  const s1::uc::Char32 chars_utf32[] = { 0x1f435, 0x1f600, 0 };
  s1::uc::String str (chars_utf32);
  const s1::uc::Char chars_utf16[] = { 0xD83D, 0xDC35, 0xD83D, 0xDE00, 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK(!str.isEmpty());
  BOOST_CHECK_EQUAL(str.length(), 4u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.countChar32(), 2u);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

// Test fromUTF8 with ASCII input
BOOST_AUTO_TEST_CASE(UTF8Simple)
{
  const char chars_utf8[] = "Hello";
  s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
  const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(str.length(), 5u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

// Test convertUTF8 with ASCII input
BOOST_AUTO_TEST_CASE(UTF8Simple_convert)
{
  const char chars_utf8[] = "Hello";
  auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
  const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 5u);
  BOOST_CHECK_EQUAL(convert_res.str, str_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.data()[convert_res.str.size()], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), 0u);
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceSuccess);
}

// Test fromUTF8 with a character encoded in 2 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded2)
{
  const char chars_utf8[] = "H\xc3\xabllo";
  s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
  const s1::uc::Char chars_utf16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(str.length(), 5u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

// Test convertUTF8 with a character encoded in 2 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded2_convert)
{
  const char chars_utf8[] = "H\xc3\xabllo";
  auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
  const s1::uc::Char chars_utf16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 5u);
  BOOST_CHECK_EQUAL(convert_res.str, str_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.data()[convert_res.str.size()], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), 0u);
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceSuccess);
}

// Test fromUTF8 with characters encoded in 3 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded3)
{
  const char chars_utf8[] = "He\xe1\xb8\xb7\xe1\xb8\xb7o";
  s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
  const s1::uc::Char chars_utf16[] = { 'H', 'e', 0x1e37, 0x1e37, 'o', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(str.length(), 5u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

// Test convertUTF8 with characters encoded in 3 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded3_convert)
{
  const char chars_utf8[] = "He\xe1\xb8\xb7\xe1\xb8\xb7o";
  auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
  const s1::uc::Char chars_utf16[] = { 'H', 'e', 0x1e37, 0x1e37, 'o', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 5u);
  BOOST_CHECK_EQUAL(convert_res.str, str_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.data()[convert_res.str.size()], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), 0u);
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceSuccess);
}

// Test fromUTF8 with characters encoded in 4 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded4)
{
  const char chars_utf8[] = "\xf0\x9f\x98\x80 \xf0\x9f\x92\xa9";
  s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
  const s1::uc::Char chars_utf16[] = { 0xd83d, 0xde00, ' ', 0xd83d, 0xdca9, 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(str.length(), 5u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

// Test convertUTF8 with characters encoded in 4 bytes
BOOST_AUTO_TEST_CASE(UTF8Encoded4_convert)
{
  const char chars_utf8[] = "\xf0\x9f\x98\x80 \xf0\x9f\x92\xa9";
  auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
  const s1::uc::Char chars_utf16[] = { 0xd83d, 0xde00, ' ', 0xd83d, 0xdca9, 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 5u);
  BOOST_CHECK_EQUAL(convert_res.str, str_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.data()[convert_res.str.size()], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), 0u);
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceSuccess);
}

// Test fromUTF8 with malformed input (overlong encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed1)
{
  const char chars_utf8[] = "a\xc0\x8a" "b";
  s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
  const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(str.length(), 3u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

// Test convertUTF8 with malformed input (overlong encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed1_convert)
{
  const char chars_utf8[] = "a\xc0\x8a" "b";
  auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
  const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 3u);
  BOOST_CHECK_EQUAL(convert_res.str, str_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.data()[convert_res.str.size()], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf8+3));
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceEncodingInvalid);
}

// Test fromUTF8 with malformed input (broken encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed2)
{
  const char chars_utf8[] = "a\xc3" "b";
  s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
  const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(str.length(), 3u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

// Test convertUTF8 with malformed input (broken encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed2_convert)
{
  const char chars_utf8[] = "a\xc3" "b";
  auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
  const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 3u);
  BOOST_CHECK_EQUAL(convert_res.str, str_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.data()[convert_res.str.size()], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf8+2));
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceCharacterIncomplete);
}

// Test fromUTF8 with malformed input (broken encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed2b)
{
  const char chars_utf8[] = "a\xab" "b";
  s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
  const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(str.length(), 3u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

// Test convertUTF8 with malformed input (broken encoding)
BOOST_AUTO_TEST_CASE(UTF8Malformed2b_convert)
{
  const char chars_utf8[] = "a\xab" "b";
  auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
  const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 3u);
  BOOST_CHECK_EQUAL(convert_res.str, str_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.data()[convert_res.str.size()], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf8+2));
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceCharacterIncomplete);
}

// Test fromUTF8 with malformed input (encoded surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed3)
{
  const char chars_utf8[] = "a\xed\xa0\x80" "b";
  s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
  const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(str.length(), 3u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

// Test convertUTF8 with malformed input (encoded surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed3_convert)
{
  const char chars_utf8[] = "a\xed\xa0\x80" "b";
  auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
  const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 3u);
  BOOST_CHECK_EQUAL(convert_res.str, str_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.data()[convert_res.str.size()], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf8+4));
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceCharacterInvalid);
}

// Test fromUTF8 with malformed input (encoded surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed4)
{
  const char chars_utf8[] = "a\xed\xb0\x80" "b";
  s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
  const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(str.length(), 3u);
  BOOST_CHECK_EQUAL(str, str_utf16);
  BOOST_CHECK_EQUAL(str.data()[str.size()], 0);
}

// Test convertUTF8 with malformed input (encoded surrogate half)
BOOST_AUTO_TEST_CASE(UTF8Malformed4_convert)
{
  const char chars_utf8[] = "a\xed\xb0\x80" "b";
  auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
  const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
  s1::uc::String str_utf16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 3u);
  BOOST_CHECK_EQUAL(convert_res.str, str_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.data()[convert_res.str.size()], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf8+4));
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceCharacterInvalid);
}

// Test convertUTF16 with some basic input
BOOST_AUTO_TEST_CASE(UTF16_convert)
{
  const s1::uc::Char16 chars_utf16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
  auto convert_res = s1::uc::String::convertUTF16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 5u);
  BOOST_CHECK_EQUAL(convert_res.str.data()[0], 'H');
  BOOST_CHECK_EQUAL(convert_res.str.data()[1], 0xeb);
  BOOST_CHECK_EQUAL(convert_res.str.data()[2], 'l');
  BOOST_CHECK_EQUAL(convert_res.str.data()[3], 'l');
  BOOST_CHECK_EQUAL(convert_res.str.data()[4], 'o');
  BOOST_CHECK_EQUAL(convert_res.str.data()[5], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), 0u);
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceSuccess);
}

// Test convertUTF16 with some input with surrogates
BOOST_AUTO_TEST_CASE(UTF16Surrogates_convert)
{
  const s1::uc::Char16 chars_utf16[] = { 'x', 0xd83d, 0xde00, 'y', 0 };
  auto convert_res = s1::uc::String::convertUTF16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 4u);
  BOOST_CHECK_EQUAL(convert_res.str.data()[0], 'x');
  BOOST_CHECK_EQUAL(convert_res.str.data()[1], 0xd83d);
  BOOST_CHECK_EQUAL(convert_res.str.data()[2], 0xde00);
  BOOST_CHECK_EQUAL(convert_res.str.data()[3], 'y');
  BOOST_CHECK_EQUAL(convert_res.str.data()[4], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), 0u);
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceSuccess);
}

// Test convertUTF16 with some malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF16Malformed1_convert)
{
  const s1::uc::Char16 chars_utf16[] = { 'x', 0xd83d, 'y', 0 };
  auto convert_res = s1::uc::String::convertUTF16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 3u);
  BOOST_CHECK_EQUAL(convert_res.str.data()[0], 'x');
  BOOST_CHECK_EQUAL(convert_res.str.data()[1], 0xfffd);
  BOOST_CHECK_EQUAL(convert_res.str.data()[2], 'y');
  BOOST_CHECK_EQUAL(convert_res.str.data()[3], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf16+2));
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceCharacterIncomplete);
}

// Test convertUTF16 with some malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF16Malformed2_convert)
{
  const s1::uc::Char16 chars_utf16[] = { 'x', 0xde00, 'y', 0 };
  auto convert_res = s1::uc::String::convertUTF16 (chars_utf16);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 3u);
  BOOST_CHECK_EQUAL(convert_res.str.data()[0], 'x');
  BOOST_CHECK_EQUAL(convert_res.str.data()[1], 0xfffd);
  BOOST_CHECK_EQUAL(convert_res.str.data()[2], 'y');
  BOOST_CHECK_EQUAL(convert_res.str.data()[3], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf16+2));
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceCharacterInvalid);
}

// Test convertUTF32 with some basic input
BOOST_AUTO_TEST_CASE(UTF32_convert)
{
  const s1::uc::Char32 chars_utf32[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
  auto convert_res = s1::uc::String::convertUTF32 (chars_utf32);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 5u);
  BOOST_CHECK_EQUAL(convert_res.str.data()[0], 'H');
  BOOST_CHECK_EQUAL(convert_res.str.data()[1], 0xeb);
  BOOST_CHECK_EQUAL(convert_res.str.data()[2], 'l');
  BOOST_CHECK_EQUAL(convert_res.str.data()[3], 'l');
  BOOST_CHECK_EQUAL(convert_res.str.data()[4], 'o');
  BOOST_CHECK_EQUAL(convert_res.str.data()[5], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), 0u);
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceSuccess);
}

// Test convertUTF32 with some malformed input (surrogates)
BOOST_AUTO_TEST_CASE(UTF32Malformed1_convert)
{
  const s1::uc::Char32 chars_utf32[] = { 'x', 0xd83d, 0xde00, 'y', 0 };
  auto convert_res = s1::uc::String::convertUTF32 (chars_utf32);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 4u);
  BOOST_CHECK_EQUAL(convert_res.str.data()[0], 'x');
  BOOST_CHECK_EQUAL(convert_res.str.data()[1], 0xfffd);
  BOOST_CHECK_EQUAL(convert_res.str.data()[2], 0xfffd);
  BOOST_CHECK_EQUAL(convert_res.str.data()[3], 'y');
  BOOST_CHECK_EQUAL(convert_res.str.data()[4], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf32+2));
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceCharacterInvalid);
}

// Test convertUTF32 with some malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF32Malformed2_convert)
{
  const s1::uc::Char32 chars_utf32[] = { 'x', 0xd83d, 'y', 0 };
  auto convert_res = s1::uc::String::convertUTF32 (chars_utf32);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 3u);
  BOOST_CHECK_EQUAL(convert_res.str.data()[0], 'x');
  BOOST_CHECK_EQUAL(convert_res.str.data()[1], 0xfffd);
  BOOST_CHECK_EQUAL(convert_res.str.data()[2], 'y');
  BOOST_CHECK_EQUAL(convert_res.str.data()[3], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf32+2));
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceCharacterInvalid);
}

// Test convertUTF32 with some malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(UTF32Malformed3_convert)
{
  const s1::uc::Char32 chars_utf32[] = { 'x', 0xde00, 'y', 0 };
  auto convert_res = s1::uc::String::convertUTF32 (chars_utf32);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 3u);
  BOOST_CHECK_EQUAL(convert_res.str.data()[0], 'x');
  BOOST_CHECK_EQUAL(convert_res.str.data()[1], 0xfffd);
  BOOST_CHECK_EQUAL(convert_res.str.data()[2], 'y');
  BOOST_CHECK_EQUAL(convert_res.str.data()[3], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf32+2));
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceCharacterInvalid);
}

// Test convertUTF32 with some malformed input (invalid code point)
BOOST_AUTO_TEST_CASE(UTF32Malformed4_convert)
{
  const s1::uc::Char32 chars_utf32[] = { 'x', 0xffffff, 'y', 0 };
  auto convert_res = s1::uc::String::convertUTF32 (chars_utf32);
  BOOST_CHECK_EQUAL(convert_res.str.length(), 3u);
  BOOST_CHECK_EQUAL(convert_res.str.data()[0], 'x');
  BOOST_CHECK_EQUAL(convert_res.str.data()[1], 0xfffd);
  BOOST_CHECK_EQUAL(convert_res.str.data()[2], 'y');
  BOOST_CHECK_EQUAL(convert_res.str.data()[3], 0);
  BOOST_CHECK_EQUAL(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf32+2));
  BOOST_CHECK_EQUAL(convert_res.error, s1::uc::String::ceCharacterInvalid);
}

// Test toUTF8String with ASCII output
BOOST_AUTO_TEST_CASE(ToUTF8Simple)
{
  const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
  const char chars_utf8[] = "Hello";
  s1::uc::String str_utf16 (chars_utf16);
  std::string str_utf8;
  str_utf16.toUTF8String (str_utf8);
  BOOST_CHECK_EQUAL(str_utf8, chars_utf8);
}

// Test toUTF8String with a character encoded in 2 bytes
BOOST_AUTO_TEST_CASE(ToUTF8Encoded2)
{
  const s1::uc::Char chars_utf16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
  const char chars_utf8[] = "H\xc3\xabllo";
  s1::uc::String str_utf16 (chars_utf16);
  std::string str_utf8;
  str_utf16.toUTF8String (str_utf8);
  BOOST_CHECK_EQUAL(str_utf8, chars_utf8);
}

// Test toUTF8String with characters encoded in 3 bytes
BOOST_AUTO_TEST_CASE(ToUTF8Encoded3)
{
  const s1::uc::Char chars_utf16[] = { 'H', 'e', 0x1e37, 0x1e37, 'o', 0 };
  const char chars_utf8[] = "He\xe1\xb8\xb7\xe1\xb8\xb7o";
  s1::uc::String str_utf16 (chars_utf16);
  std::string str_utf8;
  str_utf16.toUTF8String (str_utf8);
  BOOST_CHECK_EQUAL(str_utf8, chars_utf8);
}

// Test toUTF8String with characters encoded in 4 bytes
BOOST_AUTO_TEST_CASE(ToUTF8Encoded4)
{
  const s1::uc::Char chars_utf16[] = { 0xd83d, 0xde00, ' ', 0xd83d, 0xdca9, 0 };
  const char chars_utf8[] = "\xf0\x9f\x98\x80 \xf0\x9f\x92\xa9";
  s1::uc::String str_utf16 (chars_utf16);
  std::string str_utf8;
  str_utf16.toUTF8String (str_utf8);
  BOOST_CHECK_EQUAL(str_utf8, chars_utf8);
}

// Test toUTF8String with malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(ToUTF8Malformed1)
{
  const s1::uc::Char chars_utf16[] = { 'a', 0xd83d, 'b', 0 };
  const char chars_utf8[] = "a\xef\xbf\xbd" "b";
  s1::uc::String str_utf16 (chars_utf16);
  std::string str_utf8;
  str_utf16.toUTF8String (str_utf8);
  BOOST_CHECK_EQUAL(str_utf8, chars_utf8);
}

// Test toUTF8String with malformed input (surrogate half)
BOOST_AUTO_TEST_CASE(ToUTF8Malformed2)
{
  const s1::uc::Char chars_utf16[] = { 'a', 0xdc9a, 'b', 0 };
  const char chars_utf8[] = "a\xef\xbf\xbd" "b";
  s1::uc::String str_utf16 (chars_utf16);
  std::string str_utf8;
  str_utf16.toUTF8String (str_utf8);
  BOOST_CHECK_EQUAL(str_utf8, chars_utf8);
}

// "Normalize" some ASCII (should come out again unchanged)
BOOST_AUTO_TEST_CASE(NormalizationASCII)
{
  Char32 strASCII[127-32 + 2];
  int i;
  for (i = 32; i < 128; i++)
  {
    strASCII[i - 32] = static_cast<Char32> (i);
  }
  strASCII[i - 32] = 0;

  s1::uc::String source (s1::uc::String::fromUTF32 (strASCII, -1));
  s1::uc::String normalized (source.Normalized (s1::uc::String::normNFD));
  BOOST_CHECK_EQUAL(source, normalized);
}

BOOST_AUTO_TEST_CASE(NormalizationUCD)
{
  for (size_t i = 0; i < sizeof(normalizationTestData)/sizeof(normalizationTestData[0]); i++)
  {
    const NormalizationTestItem& item (normalizationTestData[i]);
    s1::uc::String source (s1::uc::String::fromUTF32 (item.source, -1));
    s1::uc::String normalized_expected (s1::uc::String::fromUTF32 (item.nfd, -1));
    s1::uc::String normalized (source.Normalized (s1::uc::String::normNFD));
    std::string message ((boost::format ("data item %1% mismatch") % i).str());
    BOOST_CHECK_MESSAGE(normalized == normalized_expected, message);
  }
}

// Test two strings sharing the same data
BOOST_AUTO_TEST_CASE(DataSharing1)
{
  s1::uc::String str1 (MakeLongString());
  s1::uc::String str2 (str1);
  BOOST_CHECK_EQUAL(str1, str2);
  // String should be long enough that a shared heap buffer is used
  BOOST_CHECK_EQUAL(str1.data(), str2.data());
  // Something that should cause the buffers to be un-shared
  str2.reserve (str2.length()+1);
  // Content should be same, but backed by different buffers
  BOOST_CHECK_EQUAL(str1, str2);
  BOOST_CHECK_NE(str1.data(), str2.data());
}

// Test two strings sharing the same data
BOOST_AUTO_TEST_CASE(DataSharing2)
{
  s1::uc::String str1;
  str1 = MakeLongString();
  s1::uc::String str2;
  str2 = str1;
  BOOST_CHECK_EQUAL(str1, str2);
  // String should be long enough that a shared heap buffer is used
  BOOST_CHECK_EQUAL(str1.data(), str2.data());
  // Something that should cause the buffers to be un-shared
  str2.reserve (str2.length()+1);
  // Content should be same, but backed by different buffers
  BOOST_CHECK_EQUAL(str1, str2);
  BOOST_CHECK_NE(str1.data(), str2.data());
}

// Test two strings sharing the same data
BOOST_AUTO_TEST_CASE(DataSharing3)
{
  s1::uc::String str1 (MakeLongString());
  s1::uc::String str2;
  str2.append (str1);
  BOOST_CHECK_EQUAL(str1, str2);
  // 'append()' shouldn't cause sharing (would be unintuitive anyway)
  BOOST_CHECK_NE(str1.data(), str2.data());
}

// Test two strings sharing the same data
BOOST_AUTO_TEST_CASE(DataSharing4)
{
  s1::uc::String str1;
  str1 = MakeLongString();
  str1.reserve(str1.length() + 2);
  s1::uc::String str2;
  str2 = str1;
  BOOST_CHECK_EQUAL(str1, str2);
  // String should be long enough that a shared heap buffer is used
  BOOST_CHECK_EQUAL(str1.data(), str2.data());
  // Something that should cause the buffers to be un-shared
  str2.reserve(str2.length() + 1);
  // Content should be same, but backed by different buffers
  BOOST_CHECK_EQUAL(str1, str2);
  BOOST_CHECK_NE(str1.data(), str2.data());
}

// Test string insertion
BOOST_AUTO_TEST_CASE(Insert)
{
  s1::uc::String str ("global_var");
  str.insert(0, "STOLEN");
  s1::uc::String str_expected ("STOLENglobal_var");
  BOOST_CHECK_EQUAL(str, str_expected);
}

// Test string self-append
BOOST_AUTO_TEST_CASE(SelfAppend)
{
  s1::uc::String str ("foo");
  str.append (str);
  s1::uc::String str_expected ("foofoo");
  BOOST_CHECK_EQUAL(str, str_expected);
}

// Test overflow
BOOST_AUTO_TEST_CASE(Overflow)
{
  s1::uc::String str ("foo");
  BOOST_CHECK_THROW(str.append ("dummy", s1::uc::String::max_size()-1),
                    s1::uc::StringOverflowException);
}

// Test small string swapping
BOOST_AUTO_TEST_CASE(SwapSmall)
{
  const s1::uc::String s1 ("foo");
  const s1::uc::String s2 ("bar");
  s1::uc::String s1_copy = s1;
  s1::uc::String s2_copy = s2;
  BOOST_CHECK_EQUAL (s1_copy, s1);
  BOOST_CHECK_EQUAL (s2_copy, s2);
  swap (s1_copy, s2_copy);
  BOOST_CHECK_NE (s1_copy, s2_copy);
  BOOST_CHECK_EQUAL (s2_copy, s1);
  BOOST_CHECK_EQUAL (s1_copy, s2);
}

// Test large string swapping
BOOST_AUTO_TEST_CASE(SwapLarge)
{
  const s1::uc::String s1 = MakeLongString ();
  const s1::uc::String s2 = MakeLongString2 ();
  s1::uc::String s1_copy = s1;
  s1::uc::String s2_copy = s2;
  BOOST_CHECK_EQUAL (s1_copy, s1);
  BOOST_CHECK_EQUAL (s2_copy, s2);
  swap (s1_copy, s2_copy);
  BOOST_CHECK_NE (s1_copy, s2_copy);
  BOOST_CHECK_EQUAL (s2_copy, s1);
  BOOST_CHECK_EQUAL (s1_copy, s2);
}

// Test small/large string swapping
BOOST_AUTO_TEST_CASE(SwapMixed1)
{
  const s1::uc::String s1 = "foo";
  const s1::uc::String s2 = MakeLongString2 ();
  s1::uc::String s1_copy = s1;
  s1::uc::String s2_copy = s2;
  BOOST_CHECK_EQUAL (s1_copy, s1);
  BOOST_CHECK_EQUAL (s2_copy, s2);
  swap (s1_copy, s2_copy);
  BOOST_CHECK_NE (s1_copy, s2_copy);
  BOOST_CHECK_EQUAL (s2_copy, s1);
  BOOST_CHECK_EQUAL (s1_copy, s2);
}

// Test small/large string swapping
BOOST_AUTO_TEST_CASE(SwapMixed2)
{
  const s1::uc::String s1 = MakeLongString ();
  const s1::uc::String s2 = "bar";
  s1::uc::String s1_copy = s1;
  s1::uc::String s2_copy = s2;
  BOOST_CHECK_EQUAL (s1_copy, s1);
  BOOST_CHECK_EQUAL (s2_copy, s2);
  swap (s1_copy, s2_copy);
  BOOST_CHECK_NE (s1_copy, s2_copy);
  BOOST_CHECK_EQUAL (s2_copy, s1);
  BOOST_CHECK_EQUAL (s1_copy, s2);
}

// Test null termination
BOOST_AUTO_TEST_CASE(NullTerm1)
{
  s1::uc::String str ("XXX");
  BOOST_CHECK_EQUAL (str.data ()[str.size ()], 0);
  str = "Y";
  BOOST_CHECK_EQUAL (str.data ()[str.size ()], 0);
}

BOOST_AUTO_TEST_CASE(NullTerm2)
{
  s1::uc::String str = MakeLongString ();
  BOOST_CHECK_EQUAL (str.data ()[str.size ()], 0);
}

BOOST_AUTO_TEST_CASE(ReserveExtra)
{
  const s1::uc::String::size_type longStrLen = 128;
  const s1::uc::String s1 = MakeLongString (longStrLen);
  s1::uc::String s2;
  for (s1::uc::String::size_type n = 0; n < longStrLen; n++)
  {
    s2.reserveExtra (1, 16);
    s2.append (static_cast<s1::uc::Char> ('a' + (n % 26)));
  }
  BOOST_CHECK_EQUAL (s1, s2);
}

BOOST_AUTO_TEST_CASE(ReserveExtra2)
{
  const s1::uc::String::size_type longStrLen = 128;
  const s1::uc::String s1 = MakeLongString (longStrLen);
  s1::uc::String s2;
  for (s1::uc::String::size_type n = 0; n < longStrLen; n++)
  {
    s2.reserveExtra (1, s1::uc::String::quantumGrow);
    s2.append (static_cast<s1::uc::Char> ('a' + (n % 26)));
  }
  BOOST_CHECK_EQUAL (s1, s2);
}

BOOST_AUTO_TEST_SUITE_END()
