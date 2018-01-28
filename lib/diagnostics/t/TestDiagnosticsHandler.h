/*
    Shader1
    Copyright (c) 2010-2017 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

#ifndef TESTDIAGNOSTICSHANDLER_H_
#define TESTDIAGNOSTICSHANDLER_H_

#include "diagnostics/Handler.h"

class TestDiagnosticsHandler : public s1::diagnostics::Handler
{
public:
  void LexerErrorImpl (unsigned int code, const s1::lexer::TokenLocation& location) override
  {
    lexerError.code = code;
    lexerError.location = location;
  }
  void SemanticErrorImpl (unsigned int code) override { semanticError.code = code; }
  void ParseErrorImpl (unsigned int code, ErrorInfoType info1, ErrorInfoType info2) override
  {
    ParseError parseError;
    parseError.code = code;
    if (auto encounteredToken = boost::get<const s1::lexer::Token&> (&info1))
      parseError.encounteredToken = *encounteredToken;
    if (auto expectedToken = boost::get<const s1::lexer::TokenType&> (&info2))
      parseError.expectedToken = *expectedToken;
    parseErrors.push_back (parseError);
  }

  struct {
    unsigned int code = 0;
    s1::lexer::TokenLocation location;
  } lexerError;
  struct {
    unsigned int code = 0;
  } semanticError;
  struct ParseError
  {
    unsigned int code = 0;
    s1::lexer::Token encounteredToken;
    s1::lexer::TokenType expectedToken = s1::lexer::TokenType::Invalid;
  };
  std::vector<ParseError> parseErrors;
};

#endif // TESTDIAGNOSTICSHANDLER_H_
