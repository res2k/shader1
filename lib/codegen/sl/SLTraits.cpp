/*
Shader1
Copyright (c) 2015 Frank Richter


This library is free software; you can redistribute it and/or
modify it under the terms of the wxWindows Library Licence.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Please see the wxWindows Library Licence for more details.
The licence texts can be found in the source distribution files
LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#include "base/common.h"

#include "codegen/sl/SLTraits.h"

#include <boost/multiprecision/cpp_int.hpp>

namespace s1
{
  namespace codegen
  {
    namespace sl
    {
      static inline bool IsAsciiIdentifierChar (uc::Char32 ch)
      {
        return (((ch >= 'a') && (ch <= 'z'))
                || ((ch >= 'A') && (ch <= 'Z'))
                || ((ch >= '0') && (ch <= '9'))
                || (ch == '_'));
      }

      static uc::Char rfc3492_encode_digit (size_t d)
      {
        assert(d <= 35);
        return static_cast<uc::Char> (d + 22 + 75 * (d < 26));
        /*  0..25 map to ASCII a..z or A..Z */
        /* 26..35 map to ASCII 0..9         */
      }

      enum
      {
        rfc3492_base = 36,
        rfc3492_tmin = 1,
        rfc3492_tmax = 26,
        rfc3492_skew = 38,
        rfc3492_damp = 700
      };

      typedef boost::multiprecision::cpp_int delta_type;
      static size_t rfc3492_adapt (delta_type delta, size_t numpoints, bool firsttime)
      {
        size_t k;

        if (firsttime)
          delta = delta / int (rfc3492_damp);
        else
          delta = delta >> 1;
        /* delta >> 1 is a faster way of doing delta / 2 */
        delta += delta / numpoints;

        for (k = 0;  delta > ((rfc3492_base - rfc3492_tmin) * rfc3492_tmax) / 2;  k += rfc3492_base) {
          delta /= rfc3492_base - rfc3492_tmin;
        }

        return delta_type (k + (rfc3492_base - rfc3492_tmin + 1) * delta / (delta + int (rfc3492_skew))).convert_to<size_t> ();
      }

      static inline bool IsIdentifierASCII (const uc::String& identifier)
      {
        uc::String::CharacterIterator idIt (identifier);
        if (!idIt.hasNext ()) return false;

        uc::Char32 ch = idIt.next32PostInc ();
        if (!(((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z')) || (ch == '_'))) return false;

        while (idIt.hasNext ())
        {
          uc::Char32 ch = idIt.next32PostInc ();
          if (!IsAsciiIdentifierChar (ch) && (ch != '_')) return false;
        }
        return true;
      }

      uc::String Traits::IdentifierToASCII (const uc::String& identifier, bool preferVerbatim)
      {
        // Return valid identifiers verbatim
        if (preferVerbatim && IsIdentifierASCII (identifier)) return identifier;

        uc::String basic_str;
        basic_str.reserve (identifier.length ());
        // An implementation of the punycode algorithm, see RFC 3492
        uc::String::CharacterIterator idIt (identifier);
        while (idIt.hasNext ())
        {
          uc::Char32 ch = idIt.next32PostInc ();
          // Valid ASCII identifier character?
          if (IsAsciiIdentifierChar (ch))
          {
            basic_str.append (ch);
          }
        }
        // Actual encoding, see RFC 3492 sect. 6.3
        const size_t initial_bias = 72;
        const size_t initial_n = 0;
        size_t n = initial_n;
        delta_type delta = 0;
        size_t bias = initial_bias;
        size_t h;
        size_t b;
        // let h = b = the number of basic code points in the input
        h = b = basic_str.size ();
        uc::String outStr (basic_str);
        outStr.append (uc::Char ('_'));
        const size_t num_str_chars (identifier.countChar32 ());
        while (h < num_str_chars)
        {
          // let m = the minimum {non-basic} code point >= n in the input
          size_t m = INT32_MAX;
          for (idIt.setToStart (); idIt.hasNext ();)
          {
            uc::Char32 c = idIt.next32PostInc ();;
            if (!IsAsciiIdentifierChar (c) && (size_t (c) >= n) && (size_t (c) < m))
              m = c;
          }
          delta = delta + delta_type (m - n) * delta_type (h + 1);
          n = m;
          // for each code point c in the input (in order) ...
          for (idIt.setToStart (); idIt.hasNext ();)
          {
            uc::Char32 c = idIt.next32PostInc ();;
            if ((size_t (c) < n) || (IsAsciiIdentifierChar (c)))
            {
              // increment delta
              delta++;
            }
            if (size_t (c) == n)
            {
              delta_type q = delta;
              //for k = base to infinity in steps of base 
              for (size_t k = rfc3492_base; ; k += rfc3492_base)
              {
                size_t t;
                if (k <= bias /*+ rfc3492_tmin*/)
                  t = rfc3492_tmin;
                else if (k >= bias + rfc3492_tmax)
                  t = rfc3492_tmax;
                else
                  t = k - bias;
                if (q < t) break;
                size_t digit = t + (delta_type (q - t).convert_to<size_t> () % (rfc3492_base - t));
                outStr.append (rfc3492_encode_digit (digit));
                q = (q - t) / (rfc3492_base - t);
              }
              outStr.append (rfc3492_encode_digit (q.convert_to<size_t> ()));
              bias = rfc3492_adapt (delta, h + 1, h == b);
              delta = 0;
              h++;
            }
          }
          delta++;
          n++;
        }
        return outStr;
      }

      uc::String Traits::ConvertIdentifier (const uc::String& identifier, bool preferVerbatim) const
      {
        return IdentifierToASCII (identifier, preferVerbatim);
      }

      intermediate::BasicType Traits::ConvertBasicType (semantics::BaseType type)
      {
        switch (type)
        {
        case semantics::BaseType::Invalid:
          return intermediate::BasicType::Invalid;
        case semantics::BaseType::Void:
          S1_ASSERT_NOT_REACHED (intermediate::BasicType::Int);
          break;
        case semantics::BaseType::Bool:
          return intermediate::BasicType::Bool;
        case semantics::BaseType::Int:
          return intermediate::BasicType::Int;
        case semantics::BaseType::UInt:
          return intermediate::BasicType::UInt;
        case semantics::BaseType::Float:
          return intermediate::BasicType::Float;
        }

        // Something...
        return intermediate::BasicType::Int;
      }
    } // namespace sl
  } // namespace codegen
} // namespace s1

