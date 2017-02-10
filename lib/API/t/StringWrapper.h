/*
    Shader1
    Copyright (c) 2017 Frank Richter


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

#include "base/config.h"
#include "../StringWrapper.h"

class StringWrapperTestSuite : public CxxTest::TestSuite
{
public:
  void testEmpty (void)
  {
    s1::api_impl::StringWrapper empty;
    TS_ASSERT_EQUALS (std::char_traits<char>::length (empty.GetUTF8 ()), 0);
    TS_ASSERT_EQUALS (empty.GetUTF8 ()[0], 0);
    TS_ASSERT_EQUALS (std::char_traits<char16_t>::length (empty.GetUTF16 ()), 0);
    TS_ASSERT_EQUALS (empty.GetUTF16 ()[0], 0);
    TS_ASSERT_EQUALS (std::char_traits<char32_t>::length (empty.GetUTF32 ()), 0);
    TS_ASSERT_EQUALS (empty.GetUTF32 ()[0], 0);
    TS_ASSERT_EQUALS (std::char_traits<wchar_t>::length (empty.GetWS ()), 0);
    TS_ASSERT_EQUALS (empty.GetWS ()[0], 0);
  }

  void testAscii (void)
  {
    s1::api_impl::StringWrapper hello (s1::uc::String ("Hello"));
    TS_ASSERT_EQUALS (std::char_traits<char>::length (hello.GetUTF8 ()), 5);
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[0], 'H');
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[1], 'e');
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[2], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[3], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[4], 'o');
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[5], 0);
    TS_ASSERT_EQUALS (std::char_traits<char16_t>::length (hello.GetUTF16 ()), 5);
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[0], 'H');
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[1], 'e');
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[2], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[3], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[4], 'o');
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[5], 0);
    TS_ASSERT_EQUALS (std::char_traits<char32_t>::length (hello.GetUTF32 ()), 5);
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[0], 'H');
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[1], 'e');
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[2], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[3], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[4], 'o');
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[5], 0);
    TS_ASSERT_EQUALS (std::char_traits<wchar_t>::length (hello.GetWS ()), 5);
    TS_ASSERT_EQUALS (hello.GetWS ()[0], 'H');
    TS_ASSERT_EQUALS (hello.GetWS ()[1], 'e');
    TS_ASSERT_EQUALS (hello.GetWS ()[2], 'l');
    TS_ASSERT_EQUALS (hello.GetWS ()[3], 'l');
    TS_ASSERT_EQUALS (hello.GetWS ()[4], 'o');
    TS_ASSERT_EQUALS (hello.GetWS ()[5], 0);
  }

  void testUnicode1 (void)
  {
    const s1::uc::Char chars_utf16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
    s1::api_impl::StringWrapper hello = s1::uc::String (chars_utf16);
    TS_ASSERT_EQUALS (std::char_traits<char>::length (hello.GetUTF8 ()), 6);
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[0], 'H');
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[1], (char)0xc3);
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[2], (char)0xab);
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[3], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[4], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[5], 'o');
    TS_ASSERT_EQUALS (hello.GetUTF8 ()[6], 0);
    TS_ASSERT_EQUALS (std::char_traits<char16_t>::length (hello.GetUTF16 ()), 5);
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[0], 'H');
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[1], 0xeb);
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[2], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[3], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[4], 'o');
    TS_ASSERT_EQUALS (hello.GetUTF16 ()[5], 0);
    TS_ASSERT_EQUALS (std::char_traits<char32_t>::length (hello.GetUTF32 ()), 5);
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[0], 'H');
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[1], 0xeb);
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[2], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[3], 'l');
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[4], 'o');
    TS_ASSERT_EQUALS (hello.GetUTF32 ()[5], 0);
    TS_ASSERT_EQUALS (std::char_traits<wchar_t>::length (hello.GetWS ()), 5);
    TS_ASSERT_EQUALS (hello.GetWS ()[0], 'H');
    TS_ASSERT_EQUALS (hello.GetWS ()[1], 0xeb);
    TS_ASSERT_EQUALS (hello.GetWS ()[2], 'l');
    TS_ASSERT_EQUALS (hello.GetWS ()[3], 'l');
    TS_ASSERT_EQUALS (hello.GetWS ()[4], 'o');
    TS_ASSERT_EQUALS (hello.GetWS ()[5], 0);
  }

  void testUnicode2 (void)
  {
    const s1::uc::Char chars_utf16[] = { 0xD83D, 0xDC35, 0 };
    s1::api_impl::StringWrapper monkey = s1::uc::String (chars_utf16);
    TS_ASSERT_EQUALS (std::char_traits<char>::length (monkey.GetUTF8 ()), 4);
    TS_ASSERT_EQUALS (monkey.GetUTF8 ()[0], (char)0xf0);
    TS_ASSERT_EQUALS (monkey.GetUTF8 ()[1], (char)0x9f);
    TS_ASSERT_EQUALS (monkey.GetUTF8 ()[2], (char)0x90);
    TS_ASSERT_EQUALS (monkey.GetUTF8 ()[3], (char)0xb5);
    TS_ASSERT_EQUALS (monkey.GetUTF8 ()[4], 0);
    TS_ASSERT_EQUALS (std::char_traits<char16_t>::length (monkey.GetUTF16 ()), 2);
    TS_ASSERT_EQUALS (monkey.GetUTF16 ()[0], 0xd83d);
    TS_ASSERT_EQUALS (monkey.GetUTF16 ()[1], 0xdc35);
    TS_ASSERT_EQUALS (monkey.GetUTF16 ()[2], 0);
    TS_ASSERT_EQUALS (std::char_traits<char32_t>::length (monkey.GetUTF32 ()), 1);
    TS_ASSERT_EQUALS (monkey.GetUTF32 ()[0], 0x1F435);
    TS_ASSERT_EQUALS (monkey.GetUTF32 ()[1], 0);
  }
};
