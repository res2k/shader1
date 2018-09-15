/*
    Shader1
    Copyright (c) 2018 Frank Richter


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
 * Helper: Define operator<< for s1::uc::String
 */

#ifndef PRINT_UC_STRING_H_
#define PRINT_UC_STRING_H_

#include "base/uc/String.h"
#include "base/uc/UTF16Decoder.h"

#include <iostream>

#include <boost/format.hpp>

namespace s1
{
  namespace uc
  {
    static std::ostream& operator<< (std::ostream& stream, const String& ucs)
    {
      stream << "\"";
      s1::uc::String::CharacterIterator idIt (ucs);
      while (idIt.hasNext())
      {
        s1::uc::Char32 ch = idIt.next32PostInc();
        if ((ch >= 32) && (ch < 128))
        {
          stream.put (static_cast<char> (ch));
        }
        else
        {
          stream << (boost::format ("\\u%04x") % ch);
        }
      }
      stream << "\"";

      return stream;
    }
  } // namespace uc
} // namespace s1

#endif // PRINT_UC_STRING_H_
