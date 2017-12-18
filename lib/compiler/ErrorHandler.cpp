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

#include "base/common.h"

#include "ErrorHandler.h"

#include "diagnostics/common.h"
#include "intermediate/Diagnostics.h"
#include "lexer/Lexer.h"
#include "parser/Diagnostics.h"

#include <iostream>

namespace s1
{
namespace compiler
{
void ErrorHandler::LexerErrorImpl (unsigned int code, const lexer::TokenLocation& location)
{
  // TODO: Actually do something
}

void ErrorHandler::ParseErrorImpl (unsigned int code,
                                   const lexer::Token& encounteredToken,
                                   lexer::TokenType expectedToken)
{
  const char* errorStr = diagnostics::GetIdString (code);
  if (!errorStr) errorStr = "???";
  
  std::cerr << errorStr;
  std::cerr << "; ";
  if (encounteredToken.typeOrID != lexer::Invalid)
  {
    std::cerr << "encountered: '";
    std::cerr << Lexer::GetTokenStr (encounteredToken);
    std::cerr << "'; ";
  }
  if (expectedToken != lexer::Invalid)
  {
    std::cerr << "expected: '";
    std::cerr << Lexer::GetTokenStr (expectedToken);
    std::cerr << "'; ";
  }
  std::cerr << std::endl;
}

void ErrorHandler::SemanticErrorImpl (unsigned int code)
{
  const char* errorStr = diagnostics::GetIdString (code);
  if (!errorStr) errorStr = "???";

  std::cerr << errorStr;
  std::cerr << std::endl;
}

} // namespace compiler
} // namespace s1
