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

/**\file
 * s1::uc::String overflow exception
 */
#ifndef __BASE_UC_STRINGOVERFLOWEXCEPTION_H__
#define __BASE_UC_STRINGOVERFLOWEXCEPTION_H__

#include "../Exception.h"

namespace s1
{
  namespace uc
  {
    /// Exception signalling a string overflow.
    class StringOverflowException
      : public s1::Exception // should probably eventually derive from uc::Exception
    {
    public:
      StringOverflowException ();
    };
  } // namespace uc
} // namespace s1

#endif // __BASE_UC_STRINGOVERFLOWEXCEPTION_H__
