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
public:
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
