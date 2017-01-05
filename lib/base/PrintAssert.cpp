/*
    Shader1
    Copyright (c) 2017 Frank Richter


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

#include "base/DebugMessageHandler.h"

#include <boost/container/small_vector.hpp>

#include <stdarg.h>
#include <string>

namespace s1
{
  namespace detail
  {
    static inline int assert_fmt (assert_char* buf, size_t count, const assert_char* format, ...)
    {
      int ret;
      va_list args;
      va_start (args, format);
    #if defined(_WIN32)
      ret = vswprintf (buf, count, format, args);
    #else
      ret = vsnprintf (buf, count, format, args);
    #endif
      va_end (args);
      return ret;
    }

    void PrintAssert (const assert_char* filename, int line, const assert_char* condition, const assert_char* message)
    {
      // Note: Avoiding Formatter here, in case *that* asserts as well...
      typedef std::char_traits<assert_char> assert_char_traits;
      const assert_char formatSimple[] = _S1_ASSERT_STR("Assertion '%s' failed at %s:%d");
      const assert_char formatMessage[] = _S1_ASSERT_STR("%s (Assertion '%s' failed at %s:%d)");

      size_t bufSize = assert_char_traits::length (message ? formatMessage : formatSimple)
        + assert_char_traits::length (condition)
        + assert_char_traits::length (filename)
        + charsToFormatUint
        + (message ? assert_char_traits::length (message) : 0)
        + 1;
      boost::container::small_vector<assert_char, 256> buf;
      buf.resize (bufSize, 0);

      if (message)
      {
        assert_fmt (buf.data (), bufSize, formatMessage, message, condition, filename, line);
      }
      else
      {
        assert_fmt (buf.data (), bufSize, formatSimple, condition, filename, line);
      }

      PrintDebugMessage (buf.data ());
    }
  } // namespace detail
} // namespace s1
