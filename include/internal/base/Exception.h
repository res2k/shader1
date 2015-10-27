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

/**\file
 * Exception base class, carrying a Result code
 */
#ifndef __BASE_EXCEPTION_H__
#define __BASE_EXCEPTION_H__

#include "s1/Error.h"

#include <exception>

namespace s1
{
  /**
   * Base class for for Unicode-related exceptions
   */
  class Exception : public std::exception
  {
  protected:
    ResultCode resultCode;
  public:
    /**
     * Construct a new exception object.
     */
    Exception (ResultCode code) : resultCode (code) {}

    /// Get result code.
    ResultCode code () const { return resultCode; }
  };
} // namespace s1

#endif // __BASE_EXCEPTION_H__
