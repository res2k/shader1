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

#ifndef __PARSER_EXCEPTION_H__
#define __PARSER_EXCEPTION_H__

#include "parser/Diagnostics.h"
#include "lexer/Lexer.h"

#include <exception>

namespace s1
{
  namespace parser
  {
    class Exception : public std::exception
    {
      Error code;
      Lexer::Token encounteredToken;
      Lexer::TokenType expectedToken;
    public:
      ~Exception() throw () {}
      
      Exception (Error code) : code (code), expectedToken (lexer::Invalid) {}
      Exception (Error code, const Lexer::Token& encounteredToken,
                 Lexer::TokenType expectedToken = lexer::Invalid)
       : code (code), encounteredToken (encounteredToken),
         expectedToken (expectedToken) {}
      
      Error GetCode() const { return code; }
      Lexer::Token GetEncounteredToken() const { return encounteredToken; }
      Lexer::TokenType GetExpectedToken() const { return expectedToken; }
      
      const char* what() const throw() { return "parser::Exception"; }
    };
  } // namespace parser
} // namespace s1

#endif // __PARSER_EXCEPTION_H__
