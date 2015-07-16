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
 * s1::format::Formatter format string traits.
 */
#ifndef __BASE_FORMAT_FORMATSTRINGTRAITS_H__
#define __BASE_FORMAT_FORMATSTRINGTRAITS_H__

namespace s1
{
  namespace format
  {
    /**
     * s1::format::Formatter format string traits.
     *
     * Traits must provide:
     * - typedef CharType with type of an individual character.
     */
    template<typename FormatStringType>
    struct FormatStringTraits;

    /**
     s1::format::Formatter format string traits for simple strings.
     */
    template<typename CharTypeT>
    struct FormatStringTraits<const CharTypeT*>
    {
      typedef CharTypeT CharType;
    };
  } // namespace format
} // namespace s1

#endif // __BASE_FORMAT_FORMATSTRINGTRAITS_H__
