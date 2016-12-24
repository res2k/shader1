/*
    Shader1
    Copyright (c) 2016 Frank Richter


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
 * boost::optional<> around uc::String
 */
#ifndef __BASE_UC_STRING_OPTIONAL_H__
#define __BASE_UC_STRING_OPTIONAL_H__

#include "String.h"

#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>

namespace s1
{
  namespace uc
  {
    typedef boost::optional<String> String_optional;

    static inline String_optional make_String_optional (const char* s)
    {
      if (s) return String_optional (boost::in_place (s));
      return boost::none;
    }

    static inline String_optional make_String_optional (const s1_char16* s)
    {
      if (s) return String_optional (boost::in_place (s));
      return boost::none;
    }

    static inline String_optional make_String_optional (const s1_char32* s)
    {
      if (s) return String_optional (boost::in_place (s));
      return boost::none;
    }

    static inline String_optional make_String_optional (const wchar_t* s)
    {
      if (s) return String_optional (boost::in_place (s));
      return boost::none;
    }
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_STRING_OPTIONAL_H__
