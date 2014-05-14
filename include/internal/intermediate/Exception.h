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

#ifndef __INTERMEDIATE_EXCEPTION_H__
#define __INTERMEDIATE_EXCEPTION_H__

#include "ErrorCode.h"
#include "lexer/Lexer.h"

#include <exception>

namespace s1
{
  namespace intermediate
  {
    class Exception : public std::exception
    {
      ErrorCode code;
    public:
      ~Exception() throw () {}
      
      Exception (ErrorCode code) : code (code) {}
      
      const char* what() const throw() { return "intermediate::Exception"; }
      
      ErrorCode GetCode() const { return code; }
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_EXCEPTION_H__
