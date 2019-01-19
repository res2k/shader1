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
#include "base/math_error_handler.h"

#include "base/format/Formatter.h"
#include "base/format/std_string.h"

#include <boost/convert.hpp>
#include <boost/convert/strtol.hpp>
#include <boost/math/policies/error_handling.hpp>

#include "base/format/Formatter.ipp"

namespace s1
{
  namespace detail
  {
    // Convert boost::format style placeholders (%1%) to Formatter-style placeholders ({0})
    static std::string ConvertFormat (const char* inString)
    {
      size_t len = std::char_traits<char>::length (inString);
      std::string outStr;
      outStr.reserve (len);
      bool inPlaceholder = false;
      const char* p = inString;
      const char* end = inString + len;
      const char* phStart;
      while (p < end)
      {
        if (*p == '%')
        {
          if (inPlaceholder)
          {
            inPlaceholder = false;
            phStart++;
            if (p > phStart)
            {
              if (auto formatArg = boost::convert<int> (boost::cnv::range<const char*> (phStart, p),
                                                        boost::cnv::strtol ()))
              {
                if (auto newArg = boost::convert<std::string> (*formatArg - 1, boost::cnv::strtol ()))
                {
                  outStr.push_back ('{');
                  outStr.append (*newArg);
                  outStr.push_back ('}');
                }
              }
            }
          }
          else
          {
            inPlaceholder = true;
            phStart = p;
          }
        }
        else if (!inPlaceholder)
        {
          outStr.push_back (*p);
        }
        p++;
      }
      return outStr;
    }

    void RaiseRoundingError (const char* function, const char* message, const char* typeStr, const char* valStr)
    {
      std::string functionStr;
      format::Formatter (ConvertFormat (function)) (functionStr, typeStr);
      std::string messageStr;
      format::Formatter (ConvertFormat (message)) (messageStr, valStr);

      std::string exceptStr;
      format::Formatter ("Error in {0}: {1}") (exceptStr, functionStr, messageStr);
      throw boost::math::rounding_error (exceptStr);
    }
  } // namespace detail
} // namespace s1
