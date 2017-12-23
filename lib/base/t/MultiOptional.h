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

#include "base/MultiOptional.h"

class MultiOptionalSuite : public CxxTest::TestSuite 
{
public:
  void testOneElement (void)
  {
    typedef s1::MultiOptional<int> TestType;

    {
      TestType x;
      TS_ASSERT(!x.has_value<0> ());
      TS_ASSERT_THROWS(x.value<0> (), s1::bad_optional_access);
    }
    {
      TestType x;
      x.emplace<0> (42);
      TS_ASSERT(x.has_value<0> ());
      TS_ASSERT_EQUALS(x.value<0> (), 42);
    }
    {
      TestType x;
      x.get<0>() = 42;
      TS_ASSERT(x.has_value<0> ());
      TS_ASSERT_EQUALS(x.value<0> (), 42);
    }
    {
      TestType x;
      x.emplace<0> (42);
      TS_ASSERT(x.has_value<0> ());
      x.reset<0> ();
      TS_ASSERT(!x.has_value<0> ());
    }
    {
      TestType x;
      x.emplace<0> (42);
      TS_ASSERT(x.has_value<0> ());
      TS_ASSERT_EQUALS(x.value<0> (), *(x.get<0> ()));
      TS_ASSERT_EQUALS(*(x.get<0> ()), x.value<0> ());
    }
    {
      TestType x;
      x.get<0> () = 42;
      TS_ASSERT(x.has_value<0> ());
      x.get<0> () = s1::nullopt;
      TS_ASSERT(!x.has_value<0> ());
    }
    {
      TestType x;
      x.emplace<0> (42);
      const auto& const_x = x;
      TS_ASSERT(const_x.has_value<0> ());
      TS_ASSERT_EQUALS(const_x.value<0> (), *(const_x.get<0> ()));
      TS_ASSERT_EQUALS(*(const_x.get<0> ()), const_x.value<0> ());
      TS_ASSERT_EQUALS(*(x.get<0> ()), *(const_x.get<0> ()));
    }
  }

  void testOneElementCopy (void)
  {
    typedef s1::MultiOptional<int> TestType;
    typedef s1::MultiOptional<long> TestType2;
    {
      TestType x;
      TS_ASSERT(!x.has_value<0> ());
      TestType2 y;
      y.emplace<0> (42);
      TS_ASSERT(y.has_value<0> ());
      TS_ASSERT_EQUALS(y.value<0> (), 42);
      x = y;
      TS_ASSERT(x.has_value<0> ());
      TS_ASSERT_EQUALS(x.value<0> (), 42);
    }
    {
      TestType2 y;
      y.emplace<0> (42);
      TS_ASSERT(y.has_value<0> ());
      TS_ASSERT_EQUALS(y.value<0> (), 42);
      TestType x (y);
      TS_ASSERT(x.has_value<0> ());
      TS_ASSERT_EQUALS(x.value<0> (), 42);
    }
  }

  void testTwoElements (void)
  {
    typedef s1::MultiOptional<std::string, std::wstring> TestType;

    {
      TestType x;
      TS_ASSERT(!x.has_value<0> ());
      TS_ASSERT(!x.has_value<1> ());
      TS_ASSERT_THROWS(x.value<0> (), s1::bad_optional_access);
      TS_ASSERT_THROWS(x.value<1> (), s1::bad_optional_access);
    }
    {
      TestType x;
      x.emplace<0> ("foo");
      TS_ASSERT(x.has_value<0> ());
      TS_ASSERT(!x.has_value<1> ());
      TS_ASSERT_EQUALS(x.value<0> (), "foo");
    }
    {
      TestType x;
      x.emplace<1> (L"bar");
      TS_ASSERT(!x.has_value<0> ());
      TS_ASSERT(x.has_value<1> ());
      TS_ASSERT_EQUALS(x.value<1> (), L"bar");
    }
    {
      TestType x;
      x.get<0>() = "foo";
      TS_ASSERT(x.has_value<0> ());
      TS_ASSERT(!x.has_value<1> ());
      TS_ASSERT_EQUALS(x.value<0> (), "foo");
    }
    {
      TestType x;
      x.get<0>() = std::string ("foo");
      TS_ASSERT(x.has_value<0> ());
      TS_ASSERT(!x.has_value<1> ());
      TS_ASSERT_EQUALS(x.value<0> (), "foo");
    }
    {
      TestType x;
      x.emplace<1> (L"bar");
      TS_ASSERT(!x.has_value<0> ());
      TS_ASSERT(x.has_value<1> ());
      x.reset<1> ();
      TS_ASSERT(!x.has_value<0> ());
      TS_ASSERT(!x.has_value<1> ());
    }
    {
      TestType x;
      x.emplace<1> (L"bar");
      TS_ASSERT(!x.has_value<0> ());
      TS_ASSERT(x.has_value<1> ());
      TS_ASSERT_EQUALS(x.value<1> (), *(x.get<1> ()));
      TS_ASSERT_EQUALS(*(x.get<1> ()), x.value<1> ());
    }
    {
      TestType x;
      x.get<0> () = "foo";
      TS_ASSERT(x.has_value<0> ());
      TS_ASSERT(!x.has_value<1> ());
      x.get<0> () = s1::nullopt;
      TS_ASSERT(!x.has_value<0> ());
      TS_ASSERT(!x.has_value<1> ());
    }
    {
      TestType x;
      x.emplace<1> (L"bar");
      const auto& const_x = x;
      TS_ASSERT(!const_x.has_value<0> ());
      TS_ASSERT(const_x.has_value<1> ());
      TS_ASSERT_EQUALS(const_x.value<1> (), *(const_x.get<1> ()));
      TS_ASSERT_EQUALS(*(const_x.get<1> ()), const_x.value<1> ());
      TS_ASSERT_EQUALS(*(x.get<1> ()), *(const_x.get<1> ()));
    }
  }

  void testTwoElementsCopy (void)
  {
    typedef s1::MultiOptional<std::string, std::wstring> TestType;
    typedef s1::MultiOptional<std::string, const wchar_t*> TestType2;
    {
      TestType x;
      TS_ASSERT(!x.has_value<0> ());
      TestType2 y;
      y.emplace<0> ("foo");
      TS_ASSERT(y.has_value<0> ());
      TS_ASSERT_EQUALS(y.value<0> (), "foo");
      x = y;
      TS_ASSERT(x.has_value<0> ());
      TS_ASSERT_EQUALS(x.value<0> (), "foo");
    }
    {
      TestType2 y;
      y.emplace<0> ("foo");
      TS_ASSERT(y.has_value<0> ());
      TS_ASSERT_EQUALS(y.value<0> (), "foo");
      TestType x (y);
      TS_ASSERT(x.has_value<0> ());
      TS_ASSERT_EQUALS(x.value<0> (), "foo");
    }
    {
      TestType x;
      TS_ASSERT(!x.has_value<1> ());
      TestType2 y;
      y.emplace<1> (L"bar");
      TS_ASSERT(y.has_value<1> ());
      TS_ASSERT_EQUALS(y.value<1> (), L"bar");
      x = std::move (y);
      TS_ASSERT(x.has_value<1> ());
      TS_ASSERT_EQUALS(x.value<1> (), L"bar");
    }
    {
      TestType2 y;
      y.emplace<1> (L"bar");
      TS_ASSERT(y.has_value<1> ());
      TS_ASSERT_EQUALS(y.value<1> (), L"bar");
      TestType x (std::move (y));
      TS_ASSERT(x.has_value<1> ());
      TS_ASSERT_EQUALS(x.value<1> (), L"bar");
    }
  }
};
