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

// The compiler will rightly complain about some stuff we do below. But we don't want to hear.
#ifdef _MSC_VER
  #pragma warning (push)
  #pragma warning (disable: 4244)
#endif

#include <boost/test/unit_test.hpp>

#include "base/common.h"
#include "base/format/Formatter.h"
#include "base/format/std_string.h"

#include "base/format/Formatter.ipp"

// Test ASCII formatting to std::string
BOOST_AUTO_TEST_SUITE(FormatStdString)

BOOST_AUTO_TEST_CASE(FormatterEmpty)
{
  s1::format::Formatter fmt ("");
  std::string format_result;
  fmt (format_result);
  BOOST_CHECK_EQUAL (format_result, "");
}

BOOST_AUTO_TEST_CASE(FormatterNoFmt)
{
  s1::format::Formatter fmt ("hello");
  std::string format_result;
  fmt (format_result);
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterFmtBraces)
{
  s1::format::Formatter fmt ("hel{{lo");
  std::string format_result;
  fmt (format_result);
  BOOST_CHECK_EQUAL (format_result, "hel{lo");
}

BOOST_AUTO_TEST_CASE(FormatterFmtBraces2)
{
  s1::format::Formatter fmt ("hel}}lo");
  std::string format_result;
  fmt (format_result);
  BOOST_CHECK_EQUAL (format_result, "hel}lo");
}

BOOST_AUTO_TEST_CASE(FormatterOneStr)
{
  s1::format::Formatter fmt ("he{0}o");
  std::string format_result;
  fmt (format_result, "ll");
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterOneStr2)
{
  s1::format::Formatter fmt ("he{0}o");
  std::string format_result;
  const char* arg_str ("ll");
  fmt (format_result, arg_str);
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterOneStr3)
{
  s1::format::Formatter fmt ("he{0}o");
  std::string format_result;
  std::string arg_str ("ll");
  fmt (format_result, arg_str);
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterTwoStr)
{
  s1::format::Formatter fmt ("{0}{1}");
  std::string format_result;
  fmt (format_result, "hel", "lo");
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterTwoStr2)
{
  s1::format::Formatter fmt ("{0}{1}");
  std::string format_result;
  const char* arg_str1 ("hel");
  fmt (format_result, arg_str1, "lo");
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterTwoStr3)
{
  s1::format::Formatter fmt ("{0}{1}");
  std::string format_result;
  std::string arg_str2 ("lo");
  fmt (format_result, "hel", arg_str2);
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterWStr)
{
  s1::format::Formatter fmt ("he{0}o");
  std::string format_result;
  fmt (format_result, L"ll");
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterWStr2)
{
  s1::format::Formatter fmt ("he{0}o");
  std::string format_result;
  const wchar_t* arg_str (L"ll");
  fmt (format_result, arg_str);
  BOOST_CHECK_EQUAL (format_result, "hello");
}

BOOST_AUTO_TEST_CASE(FormatterInt)
{
  s1::format::Formatter fmt ("he{0}o");
  std::string format_result;
  fmt (format_result, 11);
  BOOST_CHECK_EQUAL (format_result, "he11o");
}

BOOST_AUTO_TEST_CASE(FormatterUInt)
{
  s1::format::Formatter fmt ("he{0}o");
  std::string format_result;
  fmt (format_result, 11u);
  BOOST_CHECK_EQUAL (format_result, "he11o");
}

BOOST_AUTO_TEST_CASE(FormatterIntNeg)
{
  s1::format::Formatter fmt ("{0}");
  std::string format_result;
  fmt (format_result, -11);
  BOOST_CHECK_EQUAL (format_result, "-11");
}

BOOST_AUTO_TEST_CASE(FormatterInt32Min)
{
  s1::format::Formatter fmt ("{0}");
  std::string format_result;
  fmt (format_result, std::numeric_limits<boost::int32_t>::min());
  BOOST_CHECK_EQUAL (format_result, "-2147483648");
}

BOOST_AUTO_TEST_CASE(FormatterInt32Max)
{
  s1::format::Formatter fmt ("{0}");
  std::string format_result;
  fmt (format_result, std::numeric_limits<boost::int32_t>::max());
  BOOST_CHECK_EQUAL (format_result, "2147483647");
}

BOOST_AUTO_TEST_CASE(FormatterUInt32Min)
{
  s1::format::Formatter fmt ("{0}");
  std::string format_result;
  fmt (format_result, std::numeric_limits<boost::uint32_t>::min());
  BOOST_CHECK_EQUAL (format_result, "0");
}

BOOST_AUTO_TEST_CASE(FormatterUInt32Max)
{
  s1::format::Formatter fmt ("{0}");
  std::string format_result;
  fmt (format_result, std::numeric_limits<boost::uint32_t>::max());
  BOOST_CHECK_EQUAL (format_result, "4294967295");
}

BOOST_AUTO_TEST_SUITE_END()

#ifdef _MSC_VER
  #pragma warning (pop)
#endif
