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

#include "base/common.h"
#include "base/format/Formatter.h"
#include "base/format/uc_String.h"

#include "base/format/Formatter.tpp"

#include "print_uc_String.h"

// Test formatting to s1::uc::String
BOOST_AUTO_TEST_SUITE(FormatUcStrings)

typedef s1::format::Formatter<> MyFormatter;

BOOST_AUTO_TEST_CASE(FormatterEmpty)
{
  MyFormatter fmt ("");
  s1::uc::String format_result;
  fmt (format_result);
  BOOST_CHECK_EQUAL (format_result, "");
}

BOOST_AUTO_TEST_CASE(FormatterNoFmt)
{
  MyFormatter fmt ("hello");
  s1::uc::String format_result;
  fmt (format_result);
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterFmtBraces)
{
  MyFormatter fmt ("hel{{lo");
  s1::uc::String format_result;
  fmt (format_result);
  BOOST_CHECK_EQUAL (format_result, "hel{lo");
}

BOOST_AUTO_TEST_CASE(FormatterFmtBraces2)
{
  MyFormatter fmt ("hel}}lo");
  s1::uc::String format_result;
  fmt (format_result);
  BOOST_CHECK_EQUAL (format_result, "hel}lo");
}

BOOST_AUTO_TEST_CASE(FormatterOneStr)
{
  MyFormatter fmt ("he{0}o");
  s1::uc::String format_result;
  fmt (format_result, "ll");
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterOneStr2)
{
  MyFormatter fmt ("he{0}o");
  s1::uc::String format_result;
  s1::uc::Char arg_str[] = {'l', 'l', 0};
  fmt (format_result, arg_str);
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterOneStr3)
{
  MyFormatter fmt ("he{0}o");
  s1::uc::String format_result;
  s1::uc::String arg_str ("ll");
  fmt (format_result, arg_str);
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterTwoStr)
{
  MyFormatter fmt ("{0}{1}");
  s1::uc::String format_result;
  fmt (format_result, "hel", "lo");
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterTwoStr2)
{
  MyFormatter fmt ("{0}{1}");
  s1::uc::String format_result;
  s1::uc::Char arg_str1[] = {'h', 'e', 'l', 0};
  fmt (format_result, arg_str1, "lo");
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterTwoStr3)
{
  MyFormatter fmt ("{0}{1}");
  s1::uc::String format_result;
  s1::uc::String arg_str2 ("lo");
  fmt (format_result, "hel", arg_str2);
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterWStr)
{
  MyFormatter fmt ("he{0}o");
  s1::uc::String format_result;
  fmt (format_result, L"ll");
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterWStr2)
{
  MyFormatter fmt ("he{0}o");
  s1::uc::String format_result;
  const wchar_t* arg_str (L"ll");
  fmt (format_result, arg_str);
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterInt)
{
  MyFormatter fmt ("he{0}o");
  s1::uc::String format_result;
  fmt (format_result, 11);
  BOOST_CHECK_EQUAL (format_result, "he11o");
}

BOOST_AUTO_TEST_CASE(FormatterUInt)
{
  MyFormatter fmt ("he{0}o");
  s1::uc::String format_result;
  fmt (format_result, 11u);
  BOOST_CHECK_EQUAL (format_result, "he11o");
}

BOOST_AUTO_TEST_CASE(FormatterIntNeg)
{
  MyFormatter fmt ("{0}");
  s1::uc::String format_result;
  fmt (format_result, -11);
  BOOST_CHECK_EQUAL (format_result, "-11");
}

BOOST_AUTO_TEST_CASE(FormatterInt32Min)
{
  MyFormatter fmt ("{0}");
  s1::uc::String format_result;
  fmt (format_result, std::numeric_limits<boost::int32_t>::min());
  BOOST_CHECK_EQUAL (format_result, "-2147483648");
}

BOOST_AUTO_TEST_CASE(FormatterInt32Max)
{
  MyFormatter fmt ("{0}");
  s1::uc::String format_result;
  fmt (format_result, std::numeric_limits<boost::int32_t>::max());
  BOOST_CHECK_EQUAL (format_result, "2147483647");
}

BOOST_AUTO_TEST_CASE(FormatterUInt32Min)
{
  MyFormatter fmt ("{0}");
  s1::uc::String format_result;
  fmt (format_result, std::numeric_limits<boost::uint32_t>::min());
  BOOST_CHECK_EQUAL (format_result, "0");
}

BOOST_AUTO_TEST_CASE(FormatterUInt32Max)
{
  MyFormatter fmt ("{0}");
  s1::uc::String format_result;
  fmt (format_result, std::numeric_limits<boost::uint32_t>::max());
  BOOST_CHECK_EQUAL (format_result, "4294967295");
}

BOOST_AUTO_TEST_SUITE_END()
