/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


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

#include <boost/format.hpp>

#include <string>

#include "base/uc/String.h"
#include "base/uc/StringOverflowException.h"
#include "uc_String_Trait.h"

typedef s1::uc::Char32 Char32;

#include "data/NormalizationTestData.inc"

class StringTestSuite : public CxxTest::TestSuite 
{
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
public:
  void testEmpty1 (void)
  {
    s1::uc::String empty;
    TS_ASSERT(empty.isEmpty());
    TS_ASSERT_EQUALS(empty.length(), 0);
    TS_ASSERT_EQUALS(empty.size(), 0);
    TS_ASSERT_EQUALS(empty.data()[empty.size()], 0);
  }

  void testEmpty2 (void)
  {
    s1::uc::String empty ("");
    TS_ASSERT(empty.isEmpty());
    TS_ASSERT_EQUALS(empty.length(), 0);
    TS_ASSERT_EQUALS(empty.size(), 0);
    TS_ASSERT_EQUALS(empty.data()[empty.size()], 0);
  }

  void testEmpty3 (void)
  {
    const s1::uc::Char nullStr[] = { 0 };
    s1::uc::String empty (nullStr);
    TS_ASSERT(empty.isEmpty());
    TS_ASSERT_EQUALS(empty.length(), 0);
    TS_ASSERT_EQUALS(empty.size(), 0);
    TS_ASSERT_EQUALS(empty.data()[empty.size()], 0);
  }

  void testOneChar1 (void)
  {
    s1::uc::String str ("a");
    TS_ASSERT(!str.isEmpty());
    TS_ASSERT_EQUALS(str.length(), 1);
    TS_ASSERT_EQUALS(str.data()[0], 'a');
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  void testOneChar2 (void)
  {
    const s1::uc::Char aStr[] = { 'a', 0 };
    s1::uc::String str (aStr);
    TS_ASSERT(!str.isEmpty());
    TS_ASSERT_EQUALS(str.length(), 1);
    TS_ASSERT_EQUALS(str.data()[0], 'a');
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  void testOneChar3 (void)
  {
    s1::uc::String str ('a');
    TS_ASSERT(!str.isEmpty());
    TS_ASSERT_EQUALS(str.length(), 1);
    TS_ASSERT_EQUALS(str.data()[0], 'a');
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  void testMultiChar1 (void)
  {
    s1::uc::String str ("Hello");
    const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 5);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  void testMultiChar2 (void)
  {
    const s1::uc::Char32 chars_utf32[] = { 'H', 'e', 'l', 'l', 'o', 0 };
    s1::uc::String str (chars_utf32);
    const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 5);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  void testUTF32Single (void)
  {
    s1::uc::String str (0x1f435);
    const s1::uc::Char chars_utf16[] = { 0xD83D, 0xDC35, 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT(!str.isEmpty());
    TS_ASSERT_EQUALS(str.length(), 2);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.countChar32(), 1);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  void testUTF32Multi (void)
  {
    const s1::uc::Char32 chars_utf32[] = { 0x1f435, 0x1f600, 0 };
    s1::uc::String str (chars_utf32);
    const s1::uc::Char chars_utf16[] = { 0xD83D, 0xDC35, 0xD83D, 0xDE00, 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT(!str.isEmpty());
    TS_ASSERT_EQUALS(str.length(), 4);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.countChar32(), 2);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }
  
  // Test fromUTF8 with ASCII input
  void testUTF8Simple ()
  {
    const char chars_utf8[] = "Hello";
    s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
    const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 5);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  // Test convertUTF8 with ASCII input
  void testUTF8Simple_convert ()
  {
    const char chars_utf8[] = "Hello";
    auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
    const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 5);
    TS_ASSERT_EQUALS(convert_res.str, str_utf16);
    TS_ASSERT_EQUALS(convert_res.str.data()[convert_res.str.size()], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), 0);
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceSuccess);
  }

  // Test fromUTF8 with a character encoded in 2 bytes
  void testUTF8Encoded2 ()
  {
    const char chars_utf8[] = "H\xc3\xabllo";
    s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
    const s1::uc::Char chars_utf16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 5);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  // Test convertUTF8 with a character encoded in 2 bytes
  void testUTF8Encoded2_convert ()
  {
    const char chars_utf8[] = "H\xc3\xabllo";
    auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
    const s1::uc::Char chars_utf16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 5);
    TS_ASSERT_EQUALS(convert_res.str, str_utf16);
    TS_ASSERT_EQUALS(convert_res.str.data()[convert_res.str.size()], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), 0);
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceSuccess);
  }

  // Test fromUTF8 with characters encoded in 3 bytes
  void testUTF8Encoded3 ()
  {
    const char chars_utf8[] = "He\xe1\xb8\xb7\xe1\xb8\xb7o";
    s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
    const s1::uc::Char chars_utf16[] = { 'H', 'e', 0x1e37, 0x1e37, 'o', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 5);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  // Test convertUTF8 with characters encoded in 3 bytes
  void testUTF8Encoded3_convert ()
  {
    const char chars_utf8[] = "He\xe1\xb8\xb7\xe1\xb8\xb7o";
    auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
    const s1::uc::Char chars_utf16[] = { 'H', 'e', 0x1e37, 0x1e37, 'o', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 5);
    TS_ASSERT_EQUALS(convert_res.str, str_utf16);
    TS_ASSERT_EQUALS(convert_res.str.data()[convert_res.str.size()], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), 0);
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceSuccess);
  }

  // Test fromUTF8 with characters encoded in 4 bytes
  void testUTF8Encoded4 ()
  {
    const char chars_utf8[] = "\xf0\x9f\x98\x80 \xf0\x9f\x92\xa9";
    s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
    const s1::uc::Char chars_utf16[] = { 0xd83d, 0xde00, ' ', 0xd83d, 0xdca9, 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 5);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  // Test convertUTF8 with characters encoded in 4 bytes
  void testUTF8Encoded4_convert ()
  {
    const char chars_utf8[] = "\xf0\x9f\x98\x80 \xf0\x9f\x92\xa9";
    auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
    const s1::uc::Char chars_utf16[] = { 0xd83d, 0xde00, ' ', 0xd83d, 0xdca9, 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 5);
    TS_ASSERT_EQUALS(convert_res.str, str_utf16);
    TS_ASSERT_EQUALS(convert_res.str.data()[convert_res.str.size()], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), 0);
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceSuccess);
  }

  // Test fromUTF8 with malformed input (overlong encoding)
  void testUTF8Malformed1 ()
  {
    const char chars_utf8[] = "a\xc0\x8a" "b";
    s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
    const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 3);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  // Test convertUTF8 with malformed input (overlong encoding)
  void testUTF8Malformed1_convert ()
  {
    const char chars_utf8[] = "a\xc0\x8a" "b";
    auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
    const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 3);
    TS_ASSERT_EQUALS(convert_res.str, str_utf16);
    TS_ASSERT_EQUALS(convert_res.str.data()[convert_res.str.size()], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf8+3));
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceEncodingInvalid);
  }

  // Test fromUTF8 with malformed input (broken encoding)
  void testUTF8Malformed2 ()
  {
    const char chars_utf8[] = "a\xc3" "b";
    s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
    const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 3);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  // Test convertUTF8 with malformed input (broken encoding)
  void testUTF8Malformed2_convert ()
  {
    const char chars_utf8[] = "a\xc3" "b";
    auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
    const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 3);
    TS_ASSERT_EQUALS(convert_res.str, str_utf16);
    TS_ASSERT_EQUALS(convert_res.str.data()[convert_res.str.size()], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf8+2));
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceCharacterIncomplete);
  }

  // Test fromUTF8 with malformed input (broken encoding)
  void testUTF8Malformed2b ()
  {
    const char chars_utf8[] = "a\xab" "b";
    s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
    const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 3);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  // Test convertUTF8 with malformed input (broken encoding)
  void testUTF8Malformed2b_convert ()
  {
    const char chars_utf8[] = "a\xab" "b";
    auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
    const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 3);
    TS_ASSERT_EQUALS(convert_res.str, str_utf16);
    TS_ASSERT_EQUALS(convert_res.str.data()[convert_res.str.size()], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf8+2));
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceCharacterIncomplete);
  }

  // Test fromUTF8 with malformed input (encoded surrogate half)
  void testUTF8Malformed3 ()
  {
    const char chars_utf8[] = "a\xed\xa0\x80" "b";
    s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
    const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 3);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  // Test convertUTF8 with malformed input (encoded surrogate half)
  void testUTF8Malformed3_convert ()
  {
    const char chars_utf8[] = "a\xed\xa0\x80" "b";
    auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
    const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 3);
    TS_ASSERT_EQUALS(convert_res.str, str_utf16);
    TS_ASSERT_EQUALS(convert_res.str.data()[convert_res.str.size()], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf8+4));
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceCharacterInvalid);
  }

  // Test fromUTF8 with malformed input (encoded surrogate half)
  void testUTF8Malformed4 ()
  {
    const char chars_utf8[] = "a\xed\xb0\x80" "b";
    s1::uc::String str (s1::uc::String::fromUTF8 (chars_utf8));
    const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 3);
    TS_ASSERT_EQUALS(str, str_utf16);
    TS_ASSERT_EQUALS(str.data()[str.size()], 0);
  }

  // Test convertUTF8 with malformed input (encoded surrogate half)
  void testUTF8Malformed4_convert ()
  {
    const char chars_utf8[] = "a\xed\xb0\x80" "b";
    auto convert_res = s1::uc::String::convertUTF8 (chars_utf8);
    const s1::uc::Char chars_utf16[] = { 'a', 0xfffd, 'b', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 3);
    TS_ASSERT_EQUALS(convert_res.str, str_utf16);
    TS_ASSERT_EQUALS(convert_res.str.data()[convert_res.str.size()], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf8+4));
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceCharacterInvalid);
  }

  // Test convertUTF16 with some basic input
  void testUTF16_convert ()
  {
    const s1::uc::Char16 chars_utf16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
    auto convert_res = s1::uc::String::convertUTF16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 5);
    TS_ASSERT_EQUALS(convert_res.str.data()[0], 'H');
    TS_ASSERT_EQUALS(convert_res.str.data()[1], 0xeb);
    TS_ASSERT_EQUALS(convert_res.str.data()[2], 'l');
    TS_ASSERT_EQUALS(convert_res.str.data()[3], 'l');
    TS_ASSERT_EQUALS(convert_res.str.data()[4], 'o');
    TS_ASSERT_EQUALS(convert_res.str.data()[5], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), 0);
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceSuccess);
  }

  // Test convertUTF16 with some input with surrogates
  void testUTF16Surrogates_convert ()
  {
    const s1::uc::Char16 chars_utf16[] = { 'x', 0xd83d, 0xde00, 'y', 0 };
    auto convert_res = s1::uc::String::convertUTF16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 4);
    TS_ASSERT_EQUALS(convert_res.str.data()[0], 'x');
    TS_ASSERT_EQUALS(convert_res.str.data()[1], 0xd83d);
    TS_ASSERT_EQUALS(convert_res.str.data()[2], 0xde00);
    TS_ASSERT_EQUALS(convert_res.str.data()[3], 'y');
    TS_ASSERT_EQUALS(convert_res.str.data()[4], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), 0);
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceSuccess);
  }

  // Test convertUTF16 with some malformed input (surrogate half)
  void testUTF16Malformed1_convert ()
  {
    const s1::uc::Char16 chars_utf16[] = { 'x', 0xd83d, 'y', 0 };
    auto convert_res = s1::uc::String::convertUTF16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 3);
    TS_ASSERT_EQUALS(convert_res.str.data()[0], 'x');
    TS_ASSERT_EQUALS(convert_res.str.data()[1], 0xfffd);
    TS_ASSERT_EQUALS(convert_res.str.data()[2], 'y');
    TS_ASSERT_EQUALS(convert_res.str.data()[3], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf16+2));
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceCharacterIncomplete);
  }

  // Test convertUTF16 with some malformed input (surrogate half)
  void testUTF16Malformed2_convert ()
  {
    const s1::uc::Char16 chars_utf16[] = { 'x', 0xde00, 'y', 0 };
    auto convert_res = s1::uc::String::convertUTF16 (chars_utf16);
    TS_ASSERT_EQUALS(convert_res.str.length(), 3);
    TS_ASSERT_EQUALS(convert_res.str.data()[0], 'x');
    TS_ASSERT_EQUALS(convert_res.str.data()[1], 0xfffd);
    TS_ASSERT_EQUALS(convert_res.str.data()[2], 'y');
    TS_ASSERT_EQUALS(convert_res.str.data()[3], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf16+2));
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceCharacterInvalid);
  }

  // Test convertUTF32 with some basic input
  void testUTF32_convert ()
  {
    const s1::uc::Char32 chars_utf32[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
    auto convert_res = s1::uc::String::convertUTF32 (chars_utf32);
    TS_ASSERT_EQUALS(convert_res.str.length(), 5);
    TS_ASSERT_EQUALS(convert_res.str.data()[0], 'H');
    TS_ASSERT_EQUALS(convert_res.str.data()[1], 0xeb);
    TS_ASSERT_EQUALS(convert_res.str.data()[2], 'l');
    TS_ASSERT_EQUALS(convert_res.str.data()[3], 'l');
    TS_ASSERT_EQUALS(convert_res.str.data()[4], 'o');
    TS_ASSERT_EQUALS(convert_res.str.data()[5], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), 0);
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceSuccess);
  }

  // Test convertUTF32 with some malformed input (surrogates)
  void testUTF32Malformed1_convert ()
  {
    const s1::uc::Char32 chars_utf32[] = { 'x', 0xd83d, 0xde00, 'y', 0 };
    auto convert_res = s1::uc::String::convertUTF32 (chars_utf32);
    TS_ASSERT_EQUALS(convert_res.str.length(), 4);
    TS_ASSERT_EQUALS(convert_res.str.data()[0], 'x');
    TS_ASSERT_EQUALS(convert_res.str.data()[1], 0xfffd);
    TS_ASSERT_EQUALS(convert_res.str.data()[2], 0xfffd);
    TS_ASSERT_EQUALS(convert_res.str.data()[3], 'y');
    TS_ASSERT_EQUALS(convert_res.str.data()[4], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf32+2));
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceCharacterInvalid);
  }

  // Test convertUTF32 with some malformed input (surrogate half)
  void testUTF32Malformed2_convert ()
  {
    const s1::uc::Char32 chars_utf32[] = { 'x', 0xd83d, 'y', 0 };
    auto convert_res = s1::uc::String::convertUTF32 (chars_utf32);
    TS_ASSERT_EQUALS(convert_res.str.length(), 3);
    TS_ASSERT_EQUALS(convert_res.str.data()[0], 'x');
    TS_ASSERT_EQUALS(convert_res.str.data()[1], 0xfffd);
    TS_ASSERT_EQUALS(convert_res.str.data()[2], 'y');
    TS_ASSERT_EQUALS(convert_res.str.data()[3], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf32+2));
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceCharacterInvalid);
  }

  // Test convertUTF32 with some malformed input (surrogate half)
  void testUTF32Malformed3_convert ()
  {
    const s1::uc::Char32 chars_utf32[] = { 'x', 0xde00, 'y', 0 };
    auto convert_res = s1::uc::String::convertUTF32 (chars_utf32);
    TS_ASSERT_EQUALS(convert_res.str.length(), 3);
    TS_ASSERT_EQUALS(convert_res.str.data()[0], 'x');
    TS_ASSERT_EQUALS(convert_res.str.data()[1], 0xfffd);
    TS_ASSERT_EQUALS(convert_res.str.data()[2], 'y');
    TS_ASSERT_EQUALS(convert_res.str.data()[3], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf32+2));
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceCharacterInvalid);
  }

  // Test convertUTF32 with some malformed input (invalid code point)
  void testUTF32Malformed4_convert ()
  {
    const s1::uc::Char32 chars_utf32[] = { 'x', 0xffffff, 'y', 0 };
    auto convert_res = s1::uc::String::convertUTF32 (chars_utf32);
    TS_ASSERT_EQUALS(convert_res.str.length(), 3);
    TS_ASSERT_EQUALS(convert_res.str.data()[0], 'x');
    TS_ASSERT_EQUALS(convert_res.str.data()[1], 0xfffd);
    TS_ASSERT_EQUALS(convert_res.str.data()[2], 'y');
    TS_ASSERT_EQUALS(convert_res.str.data()[3], 0);
    TS_ASSERT_EQUALS(uintptr_t (convert_res.invalidPos), uintptr_t (chars_utf32+2));
    TS_ASSERT_EQUALS(convert_res.error, s1::uc::String::ceCharacterInvalid);
  }

  // Test toUTF8String with ASCII output
  void testToUTF8Simple ()
  {
    const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
    const char chars_utf8[] = "Hello";
    s1::uc::String str_utf16 (chars_utf16);
    std::string str_utf8;
    str_utf16.toUTF8String (str_utf8);
    TS_ASSERT_EQUALS(str_utf8, chars_utf8);
  }

  // Test toUTF8String with a character encoded in 2 bytes
  void testToUTF8Encoded2 ()
  {
    const s1::uc::Char chars_utf16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
    const char chars_utf8[] = "H\xc3\xabllo";
    s1::uc::String str_utf16 (chars_utf16);
    std::string str_utf8;
    str_utf16.toUTF8String (str_utf8);
    TS_ASSERT_EQUALS(str_utf8, chars_utf8);
  }

  // Test toUTF8String with characters encoded in 3 bytes
  void testToUTF8Encoded3 ()
  {
    const s1::uc::Char chars_utf16[] = { 'H', 'e', 0x1e37, 0x1e37, 'o', 0 };
    const char chars_utf8[] = "He\xe1\xb8\xb7\xe1\xb8\xb7o";
    s1::uc::String str_utf16 (chars_utf16);
    std::string str_utf8;
    str_utf16.toUTF8String (str_utf8);
    TS_ASSERT_EQUALS(str_utf8, chars_utf8);
  }

  // Test toUTF8String with characters encoded in 4 bytes
  void testToUTF8Encoded4 ()
  {
    const s1::uc::Char chars_utf16[] = { 0xd83d, 0xde00, ' ', 0xd83d, 0xdca9, 0 };
    const char chars_utf8[] = "\xf0\x9f\x98\x80 \xf0\x9f\x92\xa9";
    s1::uc::String str_utf16 (chars_utf16);
    std::string str_utf8;
    str_utf16.toUTF8String (str_utf8);
    TS_ASSERT_EQUALS(str_utf8, chars_utf8);
  }

  // Test toUTF8String with malformed input (surrogate half)
  void testToUTF8Malformed1 ()
  {
    const s1::uc::Char chars_utf16[] = { 'a', 0xd83d, 'b', 0 };
    const char chars_utf8[] = "a\xef\xbf\xbd" "b";
    s1::uc::String str_utf16 (chars_utf16);
    std::string str_utf8;
    str_utf16.toUTF8String (str_utf8);
    TS_ASSERT_EQUALS(str_utf8, chars_utf8);
  }

  // Test toUTF8String with malformed input (surrogate half)
  void testToUTF8Malformed2 ()
  {
    const s1::uc::Char chars_utf16[] = { 'a', 0xdc9a, 'b', 0 };
    const char chars_utf8[] = "a\xef\xbf\xbd" "b";
    s1::uc::String str_utf16 (chars_utf16);
    std::string str_utf8;
    str_utf16.toUTF8String (str_utf8);
    TS_ASSERT_EQUALS(str_utf8, chars_utf8);
  }

  // "Normalize" some ASCII (should come out again unchanged)
  void testNormalizationASCII (void)
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
    TS_ASSERT_EQUALS(source, normalized);
  }

  void testNormalizationUCD (void)
  {
    for (size_t i = 0; i < sizeof(normalizationTestData)/sizeof(normalizationTestData[0]); i++)
    {
      const NormalizationTestItem& item (normalizationTestData[i]);
      s1::uc::String source (s1::uc::String::fromUTF32 (item.source, -1));
      s1::uc::String normalized_expected (s1::uc::String::fromUTF32 (item.nfd, -1));
      s1::uc::String normalized (source.Normalized (s1::uc::String::normNFD));
      std::string message ((boost::format ("data item %1% mismatch") % i).str());
      TSM_ASSERT_EQUALS(message.c_str(), normalized, normalized_expected);
    }
  }
  
  // Test two strings sharing the same data
  void testDataSharing1 ()
  {
    s1::uc::String str1 (MakeLongString());
    s1::uc::String str2 (str1);
    TS_ASSERT_EQUALS(str1, str2);
    // String should be long enough that a shared heap buffer is used
    TS_ASSERT_EQUALS(str1.data(), str2.data());
    // Something that should cause the buffers to be un-shared
    str2.reserve (str2.length()+1);
    // Content should be same, but backed by different buffers
    TS_ASSERT_EQUALS(str1, str2);
    TS_ASSERT_DIFFERS(str1.data(), str2.data());
  }
  
  // Test two strings sharing the same data
  void testDataSharing2 ()
  {
    s1::uc::String str1;
    str1 = MakeLongString();
    s1::uc::String str2;
    str2 = str1;
    TS_ASSERT_EQUALS(str1, str2);
    // String should be long enough that a shared heap buffer is used
    TS_ASSERT_EQUALS(str1.data(), str2.data());
    // Something that should cause the buffers to be un-shared
    str2.reserve (str2.length()+1);
    // Content should be same, but backed by different buffers
    TS_ASSERT_EQUALS(str1, str2);
    TS_ASSERT_DIFFERS(str1.data(), str2.data());
  }
  
  // Test two strings sharing the same data
  void testDataSharing3 ()
  {
    s1::uc::String str1 (MakeLongString());
    s1::uc::String str2;
    str2.append (str1);
    TS_ASSERT_EQUALS(str1, str2);
    // 'append()' shouldn't cause sharing (would be unintuitive anyway)
    TS_ASSERT_DIFFERS(str1.data(), str2.data());
  }

  // Test two strings sharing the same data
  void testDataSharing4()
  {
    s1::uc::String str1;
    str1 = MakeLongString();
    str1.reserve(str1.length() + 2);
    s1::uc::String str2;
    str2 = str1;
    TS_ASSERT_EQUALS(str1, str2);
    // String should be long enough that a shared heap buffer is used
    TS_ASSERT_EQUALS(str1.data(), str2.data());
    // Something that should cause the buffers to be un-shared
    str2.reserve(str2.length() + 1);
    // Content should be same, but backed by different buffers
    TS_ASSERT_EQUALS(str1, str2);
    TS_ASSERT_DIFFERS(str1.data(), str2.data());
  }

  // Test string insertion
  void testInsert()
  {
    s1::uc::String str ("global_var");
    str.insert(0, "STOLEN");
    s1::uc::String str_expected ("STOLENglobal_var");
    TS_ASSERT_EQUALS(str, str_expected);
  }

  // Test string self-append
  void testSelfAppend()
  {
    s1::uc::String str ("foo");
    str.append (str);
    s1::uc::String str_expected ("foofoo");
    TS_ASSERT_EQUALS(str, str_expected);
  }

  // Test overflow
  void testOverflow()
  {
    s1::uc::String str ("foo");
    TS_ASSERT_THROWS(str.append ("dummy", s1::uc::String::max_size()-1),
                     s1::uc::StringOverflowException);
  }

  // Test small string swapping
  void testSwapSmall ()
  {
    const s1::uc::String s1 ("foo");
    const s1::uc::String s2 ("bar");
    s1::uc::String s1_copy = s1;
    s1::uc::String s2_copy = s2;
    TS_ASSERT_EQUALS (s1_copy, s1);
    TS_ASSERT_EQUALS (s2_copy, s2);
    swap (s1_copy, s2_copy);
    TS_ASSERT_DIFFERS (s1_copy, s2_copy);
    TS_ASSERT_EQUALS (s2_copy, s1);
    TS_ASSERT_EQUALS (s1_copy, s2);
  }

  // Test large string swapping
  void testSwapLarge ()
  {
    const s1::uc::String s1 = MakeLongString ();
    const s1::uc::String s2 = MakeLongString2 ();
    s1::uc::String s1_copy = s1;
    s1::uc::String s2_copy = s2;
    TS_ASSERT_EQUALS (s1_copy, s1);
    TS_ASSERT_EQUALS (s2_copy, s2);
    swap (s1_copy, s2_copy);
    TS_ASSERT_DIFFERS (s1_copy, s2_copy);
    TS_ASSERT_EQUALS (s2_copy, s1);
    TS_ASSERT_EQUALS (s1_copy, s2);
  }

  // Test small/large string swapping
  void testSwapMixed1 ()
  {
    const s1::uc::String s1 = "foo";
    const s1::uc::String s2 = MakeLongString2 ();
    s1::uc::String s1_copy = s1;
    s1::uc::String s2_copy = s2;
    TS_ASSERT_EQUALS (s1_copy, s1);
    TS_ASSERT_EQUALS (s2_copy, s2);
    swap (s1_copy, s2_copy);
    TS_ASSERT_DIFFERS (s1_copy, s2_copy);
    TS_ASSERT_EQUALS (s2_copy, s1);
    TS_ASSERT_EQUALS (s1_copy, s2);
  }

  // Test small/large string swapping
  void testSwapMixed2 ()
  {
    const s1::uc::String s1 = MakeLongString ();
    const s1::uc::String s2 = "bar";
    s1::uc::String s1_copy = s1;
    s1::uc::String s2_copy = s2;
    TS_ASSERT_EQUALS (s1_copy, s1);
    TS_ASSERT_EQUALS (s2_copy, s2);
    swap (s1_copy, s2_copy);
    TS_ASSERT_DIFFERS (s1_copy, s2_copy);
    TS_ASSERT_EQUALS (s2_copy, s1);
    TS_ASSERT_EQUALS (s1_copy, s2);
  }

  // Test null termination
  void testNullTerm1 ()
  {
    s1::uc::String str ("XXX");
    TS_ASSERT_EQUALS (str.data ()[str.size ()], 0);
    str = "Y";
    TS_ASSERT_EQUALS (str.data ()[str.size ()], 0);
  }

  void testNullTerm2 ()
  {
    s1::uc::String str = MakeLongString ();
    TS_ASSERT_EQUALS (str.data ()[str.size ()], 0);
  }

  void testReserveExtra ()
  {
    const s1::uc::String::size_type longStrLen = 128;
    const s1::uc::String s1 = MakeLongString (longStrLen);
    s1::uc::String s2;
    for (s1::uc::String::size_type n = 0; n < longStrLen; n++)
    {
      s2.reserveExtra (1, 16);
      s2.append (static_cast<s1::uc::Char> ('a' + (n % 26)));
    }
    TS_ASSERT_EQUALS (s1, s2);
  }

};
