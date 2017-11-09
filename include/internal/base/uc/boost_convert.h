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
 * uc::String support for Boost.Convert
 */
#ifndef __BASE_UC_BOOST_CONVERT_HPP__
#define __BASE_UC_BOOST_CONVERT_HPP__

#include "Char.h"

#include <boost/convert/detail/char.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace cnv
{
  namespace detail
  {
    template<> struct is_char<s1::uc::Char> : mpl:: true_ {};
  }

#if BOOST_VERSION >= 106500
  template<> inline s1::uc::Char to_upper (s1::uc::Char c)
  { return static_cast<s1::uc::Char> (std::towupper (static_cast<wchar_t> (c))); }
#endif
}}


#endif // __BASE_UC_BOOST_CONVERT_HPP__
