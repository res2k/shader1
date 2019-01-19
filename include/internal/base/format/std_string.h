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
 * s1::format::Formatter support for std::string destinations.
 */
#ifndef __BASE_FORMAT_STD_STRING_H__
#define __BASE_FORMAT_STD_STRING_H__

#include "Sink.h"

#include <string>

namespace s1
{
  namespace format
  {
    template<>
    class Sink<std::string>
    {
      std::string& dest;
    public:
      typedef std::string DestType;
      typedef std::string::value_type value_type;
      Sink (std::string& dest, size_t output_size) : dest (dest)
      {
        dest.reserve (dest.size() + output_size);
      }

      void append (const value_type* s, size_t n)
      {
        dest.append (s, n);
      }

      void append (const DestType& s)
      {
        dest.append (s);
      }

      void push_back (value_type c)
      {
        dest.push_back (c);
      }
    };
  } // namespace format
} // namespace s1

#endif // __BASE_FORMAT_STD_STRING_H__
