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
#include "base/format/Formatter.tpp"
#include "base/math_error_handler.h"

#include <boost/convert.hpp>
#include <boost/convert/spirit.hpp>
#include <boost/convert/strtol.hpp>

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

      //---------------------------------------------------------------------

      template<typename T>
      typename ArgHelperFloat<T>::string_type ArgHelperFloat<T>::ConvertValue (T value)
      {
        using namespace boost;

        optional<string_type> floatStr =
          convert<string_type> (value, cnv::strtol ()(cnv::parameter::precision = std::numeric_limits<T>::digits10));
        assert (floatStr);
        return *floatStr;
      }

      template ArgHelperFloat<float>::string_type ArgHelperFloat<float>::ConvertValue(float);
    } // namespace detail
  } // namespace format
} // namespace s1
