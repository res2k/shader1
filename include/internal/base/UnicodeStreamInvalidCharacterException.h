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

#ifndef __BASE_UNICODESTREAMINVALIDCHARACTEREXCEPTION_H__
#define __BASE_UNICODESTREAMINVALIDCHARACTEREXCEPTION_H__

#include "UnicodeStreamException.h"

namespace s1
{
  /**
   * Exception thrown when UnicodeStream encounters an invalid character
   */
  class UnicodeStreamInvalidCharacterException : public UnicodeStreamException
  {
  public:
    /**
     * Construct a new exception object.
     * \param uerr The error code returned by ICU.
     */
    UnicodeStreamInvalidCharacterException (UErrorCode uerr)
     : UnicodeStreamException (uerr) {}
  };
  
} // namespace s1

#endif // __BASE_UNICODESTREAMINVALIDCHARACTEREXCEPTION_H__
