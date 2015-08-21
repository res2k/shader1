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

#include "base/common.h"

#include "base/format/Formatter.h"
#include "base/format/Formatter.txx"

#include <boost/convert.hpp>
#include <boost/convert/spirit.hpp>

namespace s1
{
  namespace format
  {
    namespace detail
    {
      typedef boost::cnv::spirit default_cnv_type;

      boost::optional<unsigned long int> ParseArgumentIndex (const char* indexStrBegin, const char* indexStrEnd)
      {
        return boost::convert<unsigned long int> (boost::cnv::range<const char*> (indexStrBegin, indexStrEnd), default_cnv_type ());
      }

      boost::optional<unsigned long int> ParseArgumentIndex (const wchar_t* indexStrBegin, const wchar_t* indexStrEnd)
      {
        return boost::convert<unsigned long int> (boost::cnv::range<const wchar_t*> (indexStrBegin, indexStrEnd), default_cnv_type ());
      }
    } // namespace detail
  } // namespace format
} // namespace s1
