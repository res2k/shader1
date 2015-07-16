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

/**\file
 * CxxTest trait for s1::uc::String display
 */
#ifndef __UC_STRING_TRAIT_H__
#define __UC_STRING_TRAIT_H__

#include <cxxtest/ValueTraits.h>

#include "base/uc/String.h"
#include <unicode/schriter.h>

#include <boost/format.hpp>

namespace CxxTest
{
  template<>
  class ValueTraits<s1::uc::String>
  {
    std::string as_string;
  public:
    ValueTraits(const s1::uc::String& str)
    {
      as_string.push_back ('"');
      StringCharacterIterator idIt (str);
      while (idIt.hasNext())
      {
        s1::uc::Char32 ch = idIt.next32PostInc();
        if ((ch >= 32) && (ch < 128))
        {
          as_string.push_back (static_cast<char> (ch));
        }
        else
        {
          as_string += (boost::format ("\\u%04x") % ch).str();
        }
      }
      as_string.push_back ('"');
    }
    const char *asString(void) const { return as_string.c_str(); }
  };
} // namespace CxxTest

#endif // __UC_STRING_TRAIT_H__
