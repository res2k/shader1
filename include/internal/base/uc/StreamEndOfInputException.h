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

#ifndef __BASE_UC_STREAMENDOFINPUTEXCEPTION_H__
#define __BASE_UC_STREAMENDOFINPUTEXCEPTION_H__

#include "StreamException.h"

namespace s1
{
namespace uc
{
  /**
   * Exception thrown when trying to read from a UnicodeStream beyond it's
   * end
   */
  class StreamEndOfInputException : public StreamException
  {
  public:
    /**
     * Construct a new exception object.
     */
    StreamEndOfInputException () : StreamException (UTF8Decoder::UTF8Decoder::drInputUnderrun) {}
    
    const char* what () const throw() { return "end of input"; }
  };
} // namespace uc
} // namespace s1

#endif // __BASE_UC_STREAMENDOFINPUTEXCEPTION_H__
