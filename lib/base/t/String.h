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
#include "uc_String_Trait.h"

typedef s1::uc::Char32 Char32;

#include "NormalizationTestData.inc"

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
public:
  void testEmpty1 (void)
  {
    s1::uc::String empty;
    TS_ASSERT(empty.isEmpty());
    TS_ASSERT_EQUALS(empty.length(), 0);
    TS_ASSERT_EQUALS(empty.size(), 0);
  }

  void testEmpty2 (void)
  {
    s1::uc::String empty ("");
    TS_ASSERT(empty.isEmpty());
    TS_ASSERT_EQUALS(empty.length(), 0);
    TS_ASSERT_EQUALS(empty.size(), 0);
  }

  void testEmpty3 (void)
  {
    const s1::uc::Char nullStr[] = { 0 };
    s1::uc::String empty (nullStr);
    TS_ASSERT(empty.isEmpty());
    TS_ASSERT_EQUALS(empty.length(), 0);
    TS_ASSERT_EQUALS(empty.size(), 0);
  }

  void testOneChar1 (void)
  {
    s1::uc::String str ("a");
    TS_ASSERT(!str.isEmpty());
    TS_ASSERT_EQUALS(str.length(), 1);
    TS_ASSERT_EQUALS(str[0], 'a');
  }

  void testOneChar2 (void)
  {
    const s1::uc::Char aStr[] = { 'a', 0 };
    s1::uc::String str (aStr);
    TS_ASSERT(!str.isEmpty());
    TS_ASSERT_EQUALS(str.length(), 1);
    TS_ASSERT_EQUALS(str[0], 'a');
  }

  void testOneChar3 (void)
  {
    s1::uc::String str ('a');
    TS_ASSERT(!str.isEmpty());
    TS_ASSERT_EQUALS(str.length(), 1);
    TS_ASSERT_EQUALS(str[0], 'a');
  }

  void testMultiChar1 (void)
  {
    s1::uc::String str ("Hello");
    const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 5);
    TS_ASSERT_EQUALS(str, str_utf16);
  }

  void testMultiChar2 (void)
  {
    const s1::uc::Char32 chars_utf32[] = { 'H', 'e', 'l', 'l', 'o', 0 };
    s1::uc::String str (chars_utf32);
    const s1::uc::Char chars_utf16[] = { 'H', 'e', 'l', 'l', 'o', 0 };
    s1::uc::String str_utf16 (chars_utf16);
    TS_ASSERT_EQUALS(str.length(), 5);
    TS_ASSERT_EQUALS(str, str_utf16);
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
    const s1::uc::Char chars_utf16[] = { 'a', 0xd83d, 'b' };
    const char chars_utf8[] = "a\xef\xbf\xbd" "b";
    s1::uc::String str_utf16 (chars_utf16);
    std::string str_utf8;
    str_utf16.toUTF8String (str_utf8);
    TS_ASSERT_EQUALS(str_utf8, chars_utf8);
  }

  // Test toUTF8String with malformed input (surrogate half)
  void testToUTF8Malformed2 ()
  {
    const s1::uc::Char chars_utf16[] = { 'a', 0xdc9a, 'b' };
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
      i++;
    }
    strASCII[i++] = 0;

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
};