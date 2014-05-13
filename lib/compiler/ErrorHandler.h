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

#ifndef __ERRORHANDLER_H__
#define __ERRORHANDLER_H__

#include "intermediate/ErrorCode.h"
#include "lexer/LexerErrorHandler.h"
#include "parser/ErrorHandler.h"

namespace s1
{
namespace compiler
{
  
class ErrorHandler : public LexerErrorHandler,
		     public parser::ErrorHandler
{
public:
  /**\name s1::LexerErrorHandler
   * @{ */
  void InputInvalidCharacter () { }
  void StrayCharacter (uc::Char32 offender)
  {
  }
  /** @} */
  
  /**\name s1::LexerErrorHandler
   * @{ */
  void ParseError (s1::parser::ErrorCode code, const s1::Lexer::Token& encounteredToken,
		   s1::Lexer::TokenType expectedToken);
  /** @} */
  
  void IntermediateError (s1::intermediate::ErrorCode code);
};

} // namespace compiler
} // namespace s1

#endif // __ERRORHANDLER_H__
