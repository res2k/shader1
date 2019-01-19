/*
    Shader1
    Copyright (c) 2019 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef BASE_STRING_VIEW_H_
#define BASE_STRING_VIEW_H_

/**\file
 * string_view definition, from \c std if available, from \c boost otherwise
 */

#include "base/config.h"

#if defined(HAVE_STRING_VIEW)
  #include <string_view>
#else
  #include <boost/config.hpp>
  #include <boost/utility/string_view.hpp>
#endif // defined(HAVE_STRING_VIEW)

namespace s1
{
#if defined(HAVE_STRING_VIEW)
  typedef std::string_view string_view;
  typedef std::wstring_view wstring_view;
#else
  typedef boost::string_view string_view;
  typedef boost::wstring_view wstring_view;
#endif // defined(HAVE_STRING_VIEW)
} // namespace s1

#endif // BASE_STRING_VIEW_H_
