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

#ifndef __BASE_UC_STREAMEXCEPTION_H__
#define __BASE_UC_STREAMEXCEPTION_H__

#include "Exception.h"
#include "UTF8Decoder.h"

namespace s1
{
namespace uc
{
  /**
   * Base class for for Stream exceptions
   */
  class StreamException : public Exception
  {
    UTF8Decoder::DecodeResult result;
  public:
    /**
     * Construct a new exception object.
     * \param result UTF8Decoder result code
     */
    StreamException (UTF8Decoder::DecodeResult result) : Exception (0), result (result) {}

    const char* what () const throw();
  };
  
} // namespace uc
} // namespace s1

#endif // __BASE_UC_STREAMEXCEPTION_H__
