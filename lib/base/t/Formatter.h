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

#include "base/common.h"
#include "base/format/Formatter.h"
#include "base/format/std_string.h"

#include "base/format/Formatter.txx"

// Test ASCII formatting to std::string
class FormatStdStringTestSuite : public CxxTest::TestSuite 
{
public:
  typedef s1::format::Formatter<> MyFormatter;

  void testFormatterEmpty (void)
  {
    MyFormatter fmt ("");
    std::string format_result;
    fmt (format_result);
    TS_ASSERT_EQUALS (format_result, "");
  }

  void testFormatterNoFmt (void)
  {
    MyFormatter fmt ("hello");
    std::string format_result;
    fmt (format_result);
    TS_ASSERT_EQUALS (format_result, "hello");
  }

  void testFormatterFmtBraces (void)
  {
    MyFormatter fmt ("hel{{lo");
    std::string format_result;
    fmt (format_result);
    TS_ASSERT_EQUALS (format_result, "hel{lo");
  }

  void testFormatterFmtBraces2 (void)
  {
    MyFormatter fmt ("hel}}lo");
    std::string format_result;
    fmt (format_result);
    TS_ASSERT_EQUALS (format_result, "hel}lo");
  }

  void testFormatterOneStr (void)
  {
    MyFormatter fmt ("he{0}o");
    std::string format_result;
    fmt (format_result, "ll");
    TS_ASSERT_EQUALS (format_result, "hello");
  }

  void testFormatterOneStr2 (void)
  {
    MyFormatter fmt ("he{0}o");
    std::string format_result;
    const char* arg_str ("ll");
    fmt (format_result, arg_str);
    TS_ASSERT_EQUALS (format_result, "hello");
  }

  void testFormatterOneStr3 (void)
  {
    MyFormatter fmt ("he{0}o");
    std::string format_result;
    std::string arg_str ("ll");
    fmt (format_result, arg_str);
    TS_ASSERT_EQUALS (format_result, "hello");
  }

  void testFormatterTwoStr (void)
  {
    MyFormatter fmt ("{0}{1}");
    std::string format_result;
    fmt (format_result, "hel", "lo");
    TS_ASSERT_EQUALS (format_result, "hello");
  }

  void testFormatterTwoStr2 (void)
  {
    MyFormatter fmt ("{0}{1}");
    std::string format_result;
    const char* arg_str1 ("hel");
    fmt (format_result, arg_str1, "lo");
    TS_ASSERT_EQUALS (format_result, "hello");
  }

  void testFormatterTwoStr3 (void)
  {
    MyFormatter fmt ("{0}{1}");
    std::string format_result;
    std::string arg_str2 ("lo");
    fmt (format_result, "hel", arg_str2);
    TS_ASSERT_EQUALS (format_result, "hello");
  }

  void testFormatterWStr (void)
  {
    MyFormatter fmt ("he{0}o");
    std::string format_result;
    fmt (format_result, L"ll");
    TS_ASSERT_EQUALS (format_result, "hello");
  }

  void testFormatterWStr2 (void)
  {
    MyFormatter fmt ("he{0}o");
    std::string format_result;
    const wchar_t* arg_str (L"ll");
    fmt (format_result, arg_str);
    TS_ASSERT_EQUALS (format_result, "hello");
  }

  void testFormatterInt (void)
  {
    MyFormatter fmt ("he{0}o");
    std::string format_result;
    fmt (format_result, 11);
    TS_ASSERT_EQUALS (format_result, "he11o");
  }

  void testFormatterUInt (void)
  {
    MyFormatter fmt ("he{0}o");
    std::string format_result;
    fmt (format_result, 11u);
    TS_ASSERT_EQUALS (format_result, "he11o");
  }

  void testFormatterIntNeg (void)
  {
    MyFormatter fmt ("{0}");
    std::string format_result;
    fmt (format_result, -11);
    TS_ASSERT_EQUALS (format_result, "-11");
  }

  void testFormatterInt32Min (void)
  {
    MyFormatter fmt ("{0}");
    std::string format_result;
    fmt (format_result, std::numeric_limits<boost::int32_t>::min());
    TS_ASSERT_EQUALS (format_result, "-2147483648");
  }

  void testFormatterInt32Max (void)
  {
    MyFormatter fmt ("{0}");
    std::string format_result;
    fmt (format_result, std::numeric_limits<boost::int32_t>::max());
    TS_ASSERT_EQUALS (format_result, "2147483647");
  }

  void testFormatterUInt32Min (void)
  {
    MyFormatter fmt ("{0}");
    std::string format_result;
    fmt (format_result, std::numeric_limits<boost::uint32_t>::min());
    TS_ASSERT_EQUALS (format_result, "0");
  }

  void testFormatterUInt32Max (void)
  {
    MyFormatter fmt ("{0}");
    std::string format_result;
    fmt (format_result, std::numeric_limits<boost::uint32_t>::max());
    TS_ASSERT_EQUALS (format_result, "4294967295");
  }
};
