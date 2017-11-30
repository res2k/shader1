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

#ifndef __PARSER_ERRORHANDLER_H__
#define __PARSER_ERRORHANDLER_H__

#include "parser/Diagnostics.h"
#include "lexer/Lexer.h"

namespace s1
{
  namespace parser
  {
    struct ErrorHandler
    {
      virtual void ParseError (Error code, const Lexer::Token& encounteredToken,
			       Lexer::TokenType expectedToken) {}
    };
  } // namespace parser
} // namespace s1

#endif // __PARSER_ERRORHANDLER_H__
