/*
Shader1
Copyright (c) 2010-2015 Frank Richter


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
* char_traits<> for cs::uc::Char
*/
#ifndef __CHARTRAITS_H__
#define __CHARTRAITS_H__

#include "base/uc/Char.h"

#include <string>
#include <wchar.h>

namespace s1
{
  namespace uc
  {
    namespace detail
    {
      template<typename C, bool IsWCharSize>
      class char_traits : public std::char_traits<C>
      {
      };

      /* Use wide functions, if appropriate. std::char_traits<> for charXX_t is usually
       * just an unoptimized default implementation */
      template<typename C>
      class char_traits<C, true> : public std::char_traits<C>
      {
        static wchar_t* w(C* p) { return reinterpret_cast<wchar_t*> (p); }
        static const wchar_t* w(const C* p) { return reinterpret_cast<const wchar_t*> (p); }
        static C* c(wchar_t* p) { return reinterpret_cast<C*> (p); }
        static const C* c(const wchar_t* p) { return reinterpret_cast<const C*> (p); }
      public:
        static C* move(C* dest, const C* src, std::size_t count)
        {
          return c(wmemmove(w(dest), w(src), count));
        }

        static C* copy(C* dest, const C* src, std::size_t count)
        {
          return c(wmemcpy(w(dest), w(src), count));
        }

        static int compare(const C* s1, const C* s2, std::size_t count)
        {
          return wmemcmp(w(s1), w(s2), count);
        }

        static std::size_t length(const C* s)
        {
          return wcslen(w(s));
        }

        static const C* find(const C* p, std::size_t count, const C& ch)
        {
          return c(wmemchr(w(p), ch, count));
        }
      };
    } // namespace detail

    template<typename C>
    class char_traits : public detail::char_traits<C, sizeof(C) == CXX_SIZEOF_WCHAR_T>
    {
    };

} // namespace uc
} // namespace s1

#endif // __CHARTRAITS_H__
