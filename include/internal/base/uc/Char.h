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
 * Unicode character types
 */
#ifndef __BASE_UC_CHAR_H__
#define __BASE_UC_CHAR_H__

#include <unicode/uchar.h>

namespace s1
{
  namespace uc
  {
    typedef UChar Char;
    typedef UChar Char16;
    typedef UChar32 Char32;

    static const Char32 InvalidChar32 = (Char32)~0u;
    static const Char32 ReplacementChar = 0xfffd;
    static const Char LeadSurrogateFirst = 0xd800;
    static const Char LeadSurrogateLast  = 0xdbff;
    static const Char TailSurrogateFirst = 0xdc00;
    static const Char TailSurrogateLast  = 0xdfff;
    static const Char MaxChar16 = 0xffff;
    static const Char32 MaxChar = 0x10ffff;

    /**\name Character class checks
     * @{ */
    /// Check whether a character is a whitespace character
    bool IsWhiteSpace (Char32 ch);
    /// Check whether a character is an ID start character
    bool IsIDStart (Char32 ch);
    /// Check whether a character is an ID continue character
    bool IsIDContinue (Char32 ch);
    /** @} */
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_CHAR_H__
