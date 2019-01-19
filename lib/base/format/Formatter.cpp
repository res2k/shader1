/*
    Shader1
    Copyright (c) 2010-2018 Frank Richter


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
#include "base/format/Formatter.ipp"
#include "base/math_error_handler.h"

#include <boost/convert.hpp>
#include "base/boost_convert_spirit.hpp"
#include <boost/convert/strtol.hpp>

#if defined(HAVE_CHARCONV_TO_CHARS_FLOAT)
#include <charconv>
#endif

namespace s1
{
  namespace format
  {
    namespace detail
    {
      typedef boost::cnv::spirit default_cnv_type;

      boost::optional<size_t> ParseArgumentIndex (const char* indexStrBegin, const char* indexStrEnd)
      {
        return boost::convert<size_t> (boost::cnv::range<const char*> (indexStrBegin, indexStrEnd), default_cnv_type ());
      }

      //---------------------------------------------------------------------

      template<typename T>
      typename ArgHelperFloat<T>::string_type ArgHelperFloat<T>::ConvertValue (T value)
      {
      #if defined(HAVE_CHARCONV_TO_CHARS_FLOAT)
        string_type floatStr;
        floatStr.resize (floatStr.max_size ());
        auto convertResult = std::to_chars (floatStr.data (), floatStr.data () + floatStr.max_size (), value);
        assert (convertResult.ec != std::errc ());
        floatStr.resize (convertResult.ptr - floatStr.data ());
        return floatStr;
      #else
        using namespace boost;

        optional<string_type> floatStr =
          convert<string_type> (value, cnv::strtol ()(cnv::parameter::precision = std::numeric_limits<T>::digits10));
        assert (floatStr);
        return *floatStr;
     #endif
      }

      template ArgHelperFloat<float>::string_type ArgHelperFloat<float>::ConvertValue(float);
    } // namespace detail

    void Formatter::ParseFormat (std::string_view format)
    {
      const char* p = format.data();
      const char* fmtEnd = p + format.size();
      const char* partStart = p;
      while (p != fmtEnd)
      {
        if (*p == '{')
        {
          p++;
          if (*p)
          {
            if (*p == '{')
            {
              // '{{' -> emit a single '{' in output
              parts.push_back (FormatPart (partStart, p - partStart));
              p++;
              partStart = p;
              continue;
            }
            else
            {
              // Part up to '{'
              if ((p - 1) > partStart)
              {
                parts.push_back (FormatPart (partStart, p - 1 - partStart));
              }
              // Parse format argument
              const char* argEnd = p;
              while (*argEnd && (*argEnd != '}')) argEnd++;
              // Parse index
              if (argEnd > p)
              {
                boost::optional<size_t> index (detail::ParseArgumentIndex (p, argEnd));
                if (!index) throw std::logic_error ("Invalid format placeholder");
                parts.push_back (FormatPart (*index));
              }
              // else: assert or so?
              // Continue after '}'
              p = argEnd;
              if (*p) p++;
              partStart = p;
              continue;
            }
          }
        }
        else if (*p == '}')
        {
          p++;
          parts.push_back (FormatPart (partStart, p - partStart));
          /* Collapse '}}' as to a single '}' for symmetry with '}'
           * (but accept lone '}' as well) */
          if (*p && *p == '}')
            p++;
          partStart = p;
          continue;
        }
        p++;
      }

      if (p > partStart)
        parts.push_back (FormatPart (partStart, p - partStart));
    }

    Formatter::Formatter (std::string_view format)
    {
      ParseFormat (format);
    }

  } // namespace format
} // namespace s1
