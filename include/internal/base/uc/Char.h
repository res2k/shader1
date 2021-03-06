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
 * Unicode-related types
 */
#ifndef __BASE_UC_CHAR_H__
#define __BASE_UC_CHAR_H__

#include "s1/defs.h"

namespace s1
{
  namespace uc
  {
    /// UTF-16 code unit
    typedef s1_char16 Char16;
    /// UTF-32 code unit
    typedef s1_char32 Char32;
    /// Type of a single code unit in strings
    typedef Char16 Char;

    static const Char32 InvalidChar32 = (Char32)~0u;
    static const Char32 ReplacementChar = 0xfffd;
    static const Char LeadSurrogateFirst = 0xd800;
    static const Char LeadSurrogateLast  = 0xdbff;
    static const Char TailSurrogateFirst = 0xdc00;
    static const Char TailSurrogateLast  = 0xdfff;
    static const Char MaxChar16 = 0xffff;
    static const Char32 MaxChar = 0x10ffff;

    /**\name Code point class checks
     * @{ */
    /// Check whether a code point is a whitespace code point
    bool IsWhiteSpace (Char32 ch);
    /// Check whether a code point is an ID start code point
    bool IsIDStart (Char32 ch);
    /// Check whether a code point is an ID continue code point
    bool IsIDContinue (Char32 ch);
    /** @} */

    /**
     * Sanitize a code point.
     * Returns \c ReplacementChar if \a ch not a valid Char16
     * value.
     */
    static inline Char32 SanitizeChar16 (Char32 ch)
    {
      if ((ch > MaxChar)
        || ((ch >= LeadSurrogateFirst) && (ch <= TailSurrogateLast)))
      {
        return ReplacementChar;
      }
      return ch;
    }
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_CHAR_H__
