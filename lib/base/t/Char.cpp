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

#include "base/uc/Char.h"

BOOST_AUTO_TEST_SUITE(Char)

BOOST_AUTO_TEST_CASE(WhiteSpace)
{
  {
    // All whitespace characters
    static const s1::uc::Char32 whitespace[] =
    {
      0x85, 0x2001, 0xc, 0x2028, 0x2008, 0x3000, 0xd, 0x2005, 0x2004, 0x2006,
      0xa, 0x2009, 0x9, 0x200a, 0x2002, 0x205f, 0xb, 0x1680, 0xa0, 0x2003,
      0x2000, 0x202f, 0x2029, 0x20, 0x2007
    };
    for (size_t i = 0; i < sizeof(whitespace)/sizeof(whitespace[0]); i++)
    {
      s1::uc::Char32 ch = whitespace[i];
      std::string message ((boost::format ("%x expected to be whitespace") % ch).str());
      BOOST_CHECK_MESSAGE(s1::uc::IsWhiteSpace (ch), message);
    }
  }

  {
    // Some non-whitespace chars
    static const s1::uc::Char32 non_whitespace[] =
    {
      // Valid chars
      '.', 'a', '.', 0x800,
      // Bogus chars
      0, 0x200000, (s1::uc::Char32)0xffffffff
    };
    for (size_t i = 0; i < sizeof(non_whitespace)/sizeof(non_whitespace[0]); i++)
    {
      s1::uc::Char32 ch = non_whitespace[i];
      std::string message ((boost::format ("%x expected to be non-whitespace") % ch).str());
      BOOST_CHECK_MESSAGE(!s1::uc::IsWhiteSpace (ch), message);
    }
  }
}

BOOST_AUTO_TEST_CASE(ID)
{
  struct CharIDProps
  {
    s1::uc::Char32 ch;
    bool isStart;
    bool isContinue;
  };
  static const CharIDProps IDTestData[] = {
    { '_',    false, true  },
    { '.',    false, false },
    { ' ',    false, false },
    { '0',    false, true  },
    { 'a',    true,  true  },
    { 0x03b1, true,  true  },
    { 0x0430, true,  true  }
  };

  for (size_t i = 0; i < sizeof(IDTestData)/sizeof(IDTestData[0]); i++)
  {
    s1::uc::Char32 ch = IDTestData[i].ch;
    bool isStart (IDTestData[i].isStart);
    bool isContinue (IDTestData[i].isContinue);
    {
      std::string message ((boost::format ("%x expected to %sbe ID start") % ch % (isStart ? "" : "not ")).str());
      BOOST_CHECK_MESSAGE(isStart == s1::uc::IsIDStart (ch), message);
    }
    {
      std::string message ((boost::format ("%x expected to %sbe ID continue") % ch % (isStart ? "" : "not ")).str());
      BOOST_CHECK_MESSAGE(isContinue == s1::uc::IsIDContinue (ch), message);
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
