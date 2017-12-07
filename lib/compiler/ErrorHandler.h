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

#include "diagnostics/Handler.h"
#include "intermediate/Diagnostics.h"
#include "lexer/LexerErrorHandler.h"
#include "parser/Diagnostics.h"

namespace s1
{
namespace compiler
{
  
class ErrorHandler : public LexerErrorHandler,
                     public diagnostics::Handler
{
public:
  /**\name s1::LexerErrorHandler
   * @{ */
  void InputInvalidCharacter (const lexer::TokenLocation&) override { }
  /** @} */
protected:
  /**\name diagnostics::Handler implementation
   * @{ */
  void ParseErrorImpl (unsigned int code,
                       const lexer::Token& encounteredToken,
                       lexer::TokenType expectedToken) override;
  void SemanticErrorImpl (unsigned int code) override;
  /** @} */
};

} // namespace compiler
} // namespace s1

#endif // __ERRORHANDLER_H__
