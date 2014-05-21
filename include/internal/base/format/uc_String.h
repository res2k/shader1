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
 * s1::format::Formatter<> support for s1::uc::String destinations.
 */
#ifndef __BASE_FORMAT_UC_STRING_H__
#define __BASE_FORMAT_UC_STRING_H__

#include "Sink.h"

#include "base/uc/String.h"

#include <string.h>

namespace s1
{
  namespace format
  {
    template<>
    class Sink<uc::String>
    {
      uc::String& dest;
      uc::Char* p_start;
      uc::Char* p;
    #ifndef NDEBUG
      uc::Char* p_end;
    #endif
    public:
      typedef uc::String DestType;
      typedef uc::Char value_type;
      Sink (uc::String& dest, size_t output_size) : dest (dest)
      {
        p_start = dest.getBuffer (dest.length() + output_size + 1);
        p = p_start + dest.length();
      #ifndef NDEBUG
        p_end = p + output_size + 1;
      #endif
      }
      ~Sink()
      {
        assert (p < p_end);
        *p = 0;
        dest.releaseBuffer (p - p_start);
      }

      void append (const char* s, size_t n)
      {
        assert (p + n < p_end);
        while (n-- > 0)
        {
          *p++ = *s++;
        }
      }

      void append (const value_type* s, size_t n)
      {
        assert (p + n < p_end);
        memcpy (p, s, n * sizeof (uc::Char));
        p += n;
      }

      void append (const DestType& s)
      {
        int n (s.length());
        assert (p + n < p_end);
        memcpy (p, s.getBuffer(), n * sizeof (uc::Char));
        p += n;
      }

      void push_back (value_type c)
      {
        assert (p < p_end);
        *p++ = c;
      }
    };
  } // namespace format
} // namespace s1

#endif // __BASE_FORMAT_UC_STRING_H__
