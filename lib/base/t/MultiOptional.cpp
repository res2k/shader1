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

#include "base/MultiOptional.h"

#include "print_wstring.h"

BOOST_AUTO_TEST_SUITE(MultiOptional)

BOOST_AUTO_TEST_CASE(OneElement)
{
  typedef s1::MultiOptional<int> TestType;

  {
    TestType x;
    BOOST_CHECK(!x.has_value<0> ());
    BOOST_CHECK_THROW(x.value<0> (), s1::bad_optional_access);
  }
  {
    TestType x;
    x.emplace<0> (42);
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK_EQUAL(x.value<0> (), 42);
  }
  {
    TestType x;
    x.get<0>() = 42;
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK_EQUAL(x.value<0> (), 42);
  }
  {
    TestType x;
    x.emplace<0> (42);
    BOOST_CHECK(x.has_value<0> ());
    x.reset<0> ();
    BOOST_CHECK(!x.has_value<0> ());
  }
  {
    TestType x;
    x.emplace<0> (42);
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK_EQUAL(x.value<0> (), *(x.get<0> ()));
    BOOST_CHECK_EQUAL(*(x.get<0> ()), x.value<0> ());
  }
  {
    TestType x;
    x.get<0> () = 42;
    BOOST_CHECK(x.has_value<0> ());
    x.get<0> () = s1::nullopt;
    BOOST_CHECK(!x.has_value<0> ());
  }
  {
    TestType x;
    x.emplace<0> (42);
    const auto& const_x = x;
    BOOST_CHECK(const_x.has_value<0> ());
    BOOST_CHECK_EQUAL(const_x.value<0> (), *(const_x.get<0> ()));
    BOOST_CHECK_EQUAL(*(const_x.get<0> ()), const_x.value<0> ());
    BOOST_CHECK_EQUAL(*(x.get<0> ()), *(const_x.get<0> ()));
  }
}

BOOST_AUTO_TEST_CASE(OneElementCopy)
{
  typedef s1::MultiOptional<int> TestType;
  typedef s1::MultiOptional<long> TestType2;
  {
    TestType x;
    BOOST_CHECK(!x.has_value<0> ());
    TestType2 y;
    y.emplace<0> (42);
    BOOST_CHECK(y.has_value<0> ());
    BOOST_CHECK_EQUAL(y.value<0> (), 42);
    x = y;
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK_EQUAL(x.value<0> (), 42);
  }
  {
    TestType2 y;
    y.emplace<0> (42);
    BOOST_CHECK(y.has_value<0> ());
    BOOST_CHECK_EQUAL(y.value<0> (), 42);
    TestType x (y);
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK_EQUAL(x.value<0> (), 42);
  }
}

BOOST_AUTO_TEST_CASE(OneElementSwap)
{
  typedef s1::MultiOptional<int> TestType;

  {
    TestType x;
    x.emplace<0> (42);
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK_EQUAL(x.value<0> (), 42);
    TestType y;
    BOOST_CHECK(!y.has_value<0> ());
    swap (x, y);
    BOOST_CHECK(!x.has_value<0> ());
    BOOST_CHECK(y.has_value<0> ());
    BOOST_CHECK_EQUAL(y.value<0> (), 42);
  }
}

BOOST_AUTO_TEST_CASE(TwoElements)
{
  typedef s1::MultiOptional<std::string, std::wstring> TestType;

  {
    TestType x;
    BOOST_CHECK(!x.has_value<0> ());
    BOOST_CHECK(!x.has_value<1> ());
    BOOST_CHECK_THROW(x.value<0> (), s1::bad_optional_access);
    BOOST_CHECK_THROW(x.value<1> (), s1::bad_optional_access);
  }
  {
    TestType x;
    x.emplace<0> ("foo");
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK(!x.has_value<1> ());
    BOOST_CHECK_EQUAL(x.value<0> (), "foo");
  }
  {
    TestType x;
    x.emplace<1> (L"bar");
    BOOST_CHECK(!x.has_value<0> ());
    BOOST_CHECK(x.has_value<1> ());
    BOOST_CHECK_EQUAL(x.value<1> (), L"bar");
  }
  {
    TestType x;
    x.get<0>() = "foo";
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK(!x.has_value<1> ());
    BOOST_CHECK_EQUAL(x.value<0> (), "foo");
  }
  {
    TestType x;
    x.get<0>() = std::string ("foo");
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK(!x.has_value<1> ());
    BOOST_CHECK_EQUAL(x.value<0> (), "foo");
  }
  {
    TestType x;
    x.emplace<1> (L"bar");
    BOOST_CHECK(!x.has_value<0> ());
    BOOST_CHECK(x.has_value<1> ());
    x.reset<1> ();
    BOOST_CHECK(!x.has_value<0> ());
    BOOST_CHECK(!x.has_value<1> ());
  }
  {
    TestType x;
    x.emplace<1> (L"bar");
    BOOST_CHECK(!x.has_value<0> ());
    BOOST_CHECK(x.has_value<1> ());
    BOOST_CHECK_EQUAL(x.value<1> (), *(x.get<1> ()));
    BOOST_CHECK_EQUAL(*(x.get<1> ()), x.value<1> ());
  }
  {
    TestType x;
    x.get<0> () = "foo";
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK(!x.has_value<1> ());
    x.get<0> () = s1::nullopt;
    BOOST_CHECK(!x.has_value<0> ());
    BOOST_CHECK(!x.has_value<1> ());
  }
  {
    TestType x;
    x.emplace<1> (L"bar");
    const auto& const_x = x;
    BOOST_CHECK(!const_x.has_value<0> ());
    BOOST_CHECK(const_x.has_value<1> ());
    BOOST_CHECK_EQUAL(const_x.value<1> (), *(const_x.get<1> ()));
    BOOST_CHECK_EQUAL(*(const_x.get<1> ()), const_x.value<1> ());
    BOOST_CHECK_EQUAL(*(x.get<1> ()), *(const_x.get<1> ()));
  }
}

BOOST_AUTO_TEST_CASE(TwoElementsCopy)
{
  typedef s1::MultiOptional<std::string, std::wstring> TestType;
  typedef s1::MultiOptional<std::string, const wchar_t*> TestType2;
  {
    TestType x;
    BOOST_CHECK(!x.has_value<0> ());
    TestType2 y;
    y.emplace<0> ("foo");
    BOOST_CHECK(y.has_value<0> ());
    BOOST_CHECK_EQUAL(y.value<0> (), "foo");
    x = y;
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK_EQUAL(x.value<0> (), "foo");
  }
  {
    TestType2 y;
    y.emplace<0> ("foo");
    BOOST_CHECK(y.has_value<0> ());
    BOOST_CHECK_EQUAL(y.value<0> (), "foo");
    TestType x (y);
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK_EQUAL(x.value<0> (), "foo");
  }
  {
    TestType x;
    BOOST_CHECK(!x.has_value<1> ());
    TestType2 y;
    y.emplace<1> (L"bar");
    BOOST_CHECK(y.has_value<1> ());
    BOOST_CHECK_EQUAL(y.value<1> (), L"bar");
    x = std::move (y);
    BOOST_CHECK(x.has_value<1> ());
    BOOST_CHECK_EQUAL(x.value<1> (), L"bar");
  }
  {
    TestType2 y;
    y.emplace<1> (L"bar");
    BOOST_CHECK(y.has_value<1> ());
    BOOST_CHECK_EQUAL(y.value<1> (), L"bar");
    TestType x (std::move (y));
    BOOST_CHECK(x.has_value<1> ());
    BOOST_CHECK_EQUAL(x.value<1> (), L"bar");
  }
}

BOOST_AUTO_TEST_CASE(TwoElementsSwap)
{
  typedef s1::MultiOptional<std::string, std::wstring> TestType;

  {
    TestType x;
    x.emplace<0> ("foo");
    BOOST_CHECK(x.has_value<0> ());
    BOOST_CHECK(!x.has_value<1> ());
    BOOST_CHECK_EQUAL(x.value<0> (), "foo");
    TestType y;
    BOOST_CHECK(!y.has_value<0> ());
    BOOST_CHECK(!y.has_value<1> ());
    swap (x, y);
    BOOST_CHECK(!x.has_value<0> ());
    BOOST_CHECK(!x.has_value<1> ());
    BOOST_CHECK(y.has_value<0> ());
    BOOST_CHECK(!y.has_value<1> ());
    BOOST_CHECK_EQUAL(y.value<0> (), "foo");
  }
  {
    TestType x;
    x.emplace<1> (L"bar");
    BOOST_CHECK(!x.has_value<0> ());
    BOOST_CHECK(x.has_value<1> ());
    BOOST_CHECK_EQUAL(x.value<1> (), L"bar");
    TestType y;
    BOOST_CHECK(!y.has_value<0> ());
    BOOST_CHECK(!y.has_value<1> ());
    swap (x, y);
    BOOST_CHECK(!x.has_value<0> ());
    BOOST_CHECK(!x.has_value<1> ());
    BOOST_CHECK(!y.has_value<0> ());
    BOOST_CHECK(y.has_value<1> ());
    BOOST_CHECK_EQUAL(y.value<1> (), L"bar");
  }
}

BOOST_AUTO_TEST_SUITE_END()
