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
 * Helper: Define operator<< for std::wstring
 */

#ifndef PRINT_WSTRING_H_
#define PRINT_WSTRING_H_

#include <iostream>
#include <string>

#include <boost/format.hpp>

namespace std
{
  static std::ostream& operator<< (std::ostream& stream, const wstring& str)
  {
    stream << "\"";
    for (const auto ch : str)
    {
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
} // namespace std

#endif // PRINT_WSTRING_H_
