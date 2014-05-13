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
    typedef UChar32 Char32;
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_CHAR_H__
