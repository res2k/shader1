/*
    Shader1
    Copyright (c) 2017-2018 Frank Richter


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

#include "base/config.h"
#include "../StringWrapper.h"

#include <type_traits>

template<typename T>
static inline typename std::make_unsigned<T>::type as_unsigned (T x)
{
  return static_cast<typename std::make_unsigned<T>::type> (x);
}

#define BOOST_CHECK_EQUAL_CH(A, B)                                     \
  BOOST_CHECK_EQUAL(static_cast<uint_least32_t> (as_unsigned (A)),     \
                    static_cast<uint_least32_t> (B))

BOOST_AUTO_TEST_SUITE(StringWrapper)

BOOST_AUTO_TEST_CASE(Empty)
{
  s1::api_impl::StringWrapper empty;
  BOOST_CHECK_EQUAL (std::char_traits<char>::length (empty.GetUTF8 ()), 0);
  BOOST_CHECK_EQUAL_CH (empty.GetUTF8 ()[0], 0);
  BOOST_CHECK_EQUAL (std::char_traits<char16_t>::length (empty.GetUTF16 ()), 0);
  BOOST_CHECK_EQUAL_CH (empty.GetUTF16 ()[0], 0);
  BOOST_CHECK_EQUAL (std::char_traits<char32_t>::length (empty.GetUTF32 ()), 0);
  BOOST_CHECK_EQUAL_CH (empty.GetUTF32 ()[0], 0);
  BOOST_CHECK_EQUAL (std::char_traits<wchar_t>::length (empty.GetWCS ()), 0);
  BOOST_CHECK_EQUAL_CH (empty.GetWCS ()[0], 0);
}

BOOST_AUTO_TEST_CASE(Ascii)
{
  s1::api_impl::StringWrapper hello (s1::uc::String ("Hello"));
  BOOST_CHECK_EQUAL (std::char_traits<char>::length (hello.GetUTF8 ()), 5);
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[0], 'H');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[1], 'e');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[2], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[3], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[4], 'o');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[5], 0);
  BOOST_CHECK_EQUAL (std::char_traits<char16_t>::length (hello.GetUTF16 ()), 5);
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[0], 'H');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[1], 'e');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[2], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[3], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[4], 'o');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[5], 0);
  BOOST_CHECK_EQUAL (std::char_traits<char32_t>::length (hello.GetUTF32 ()), 5);
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[0], 'H');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[1], 'e');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[2], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[3], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[4], 'o');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[5], 0);
  BOOST_CHECK_EQUAL (std::char_traits<wchar_t>::length (hello.GetWCS ()), 5);
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[0], 'H');
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[1], 'e');
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[2], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[3], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[4], 'o');
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[5], 0);
}

BOOST_AUTO_TEST_CASE(Unicode1)
{
  const s1::uc::Char chars_utf16[] = { 'H', 0xeb, 'l', 'l', 'o', 0 };
  s1::api_impl::StringWrapper hello = s1::uc::String (chars_utf16);
  BOOST_CHECK_EQUAL (std::char_traits<char>::length (hello.GetUTF8 ()), 6);
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[0], 'H');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[1], 0xc3);
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[2], 0xab);
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[3], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[4], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[5], 'o');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF8 ()[6], 0);
  BOOST_CHECK_EQUAL (std::char_traits<char16_t>::length (hello.GetUTF16 ()), 5);
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[0], 'H');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[1], 0xeb);
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[2], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[3], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[4], 'o');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF16 ()[5], 0);
  BOOST_CHECK_EQUAL (std::char_traits<char32_t>::length (hello.GetUTF32 ()), 5);
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[0], 'H');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[1], 0xeb);
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[2], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[3], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[4], 'o');
  BOOST_CHECK_EQUAL_CH (hello.GetUTF32 ()[5], 0);
  BOOST_CHECK_EQUAL (std::char_traits<wchar_t>::length (hello.GetWCS ()), 5);
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[0], 'H');
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[1], 0xeb);
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[2], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[3], 'l');
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[4], 'o');
  BOOST_CHECK_EQUAL_CH (hello.GetWCS ()[5], 0);
}

BOOST_AUTO_TEST_CASE(Unicode2)
{
  const s1::uc::Char chars_utf16[] = { 0xD83D, 0xDC35, 0 };
  s1::api_impl::StringWrapper monkey = s1::uc::String (chars_utf16);
  BOOST_CHECK_EQUAL (std::char_traits<char>::length (monkey.GetUTF8 ()), 4);
  BOOST_CHECK_EQUAL_CH (monkey.GetUTF8 ()[0], 0xf0);
  BOOST_CHECK_EQUAL_CH (monkey.GetUTF8 ()[1], 0x9f);
  BOOST_CHECK_EQUAL_CH (monkey.GetUTF8 ()[2], 0x90);
  BOOST_CHECK_EQUAL_CH (monkey.GetUTF8 ()[3], 0xb5);
  BOOST_CHECK_EQUAL_CH (monkey.GetUTF8 ()[4], 0);
  BOOST_CHECK_EQUAL (std::char_traits<char16_t>::length (monkey.GetUTF16 ()), 2);
  BOOST_CHECK_EQUAL_CH (monkey.GetUTF16 ()[0], 0xd83d);
  BOOST_CHECK_EQUAL_CH (monkey.GetUTF16 ()[1], 0xdc35);
  BOOST_CHECK_EQUAL_CH (monkey.GetUTF16 ()[2], 0);
  BOOST_CHECK_EQUAL (std::char_traits<char32_t>::length (monkey.GetUTF32 ()), 1);
  BOOST_CHECK_EQUAL_CH (monkey.GetUTF32 ()[0], 0x1F435);
  BOOST_CHECK_EQUAL_CH (monkey.GetUTF32 ()[1], 0);
}

BOOST_AUTO_TEST_SUITE_END()
