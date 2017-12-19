/*
    Shader1
    Copyright (c) 2017 Frank Richter


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

#include "compiler/ProgramDiagnostics.h"

#include "lexer/Lexer.h"

namespace s1
{
  struct Compiler::ProgramDiagnostics::Entry
  {
    unsigned int code;
    boost::optional<uc::String> info1;
    boost::optional<uc::String> info2;
  };

  Compiler::ProgramDiagnostics::ProgramDiagnostics () {}

  Compiler::ProgramDiagnostics::~ProgramDiagnostics () {}

  void Compiler::ProgramDiagnostics::Add (unsigned int code,
                                          boost::optional<const uc::String&> info1,
                                          boost::optional<const uc::String&> info2)
  {
    Entry newEntry;
    newEntry.code = code;
    if (info1) newEntry.info1 = *info1;
    if (info2) newEntry.info2 = *info2;
    entries.push_back (std::move (newEntry));
  }

  void Compiler::ProgramDiagnostics::LexerErrorImpl (unsigned int code, const lexer::TokenLocation& location)
  {
    // TODO: Actually use location
    Add (code);
  }

  void Compiler::ProgramDiagnostics::ParseErrorImpl (unsigned int code,
                                                     const lexer::Token& encounteredToken,
                                                     lexer::TokenType expectedToken)
  {
    uc::String encounteredTokenStr (Lexer::GetTokenStr (encounteredToken).c_str());
    uc::String expectedTokenStr (Lexer::GetTokenStr (expectedToken));
    Add (code, encounteredTokenStr, expectedTokenStr);
  }

  void Compiler::ProgramDiagnostics::SemanticErrorImpl (unsigned int code)
  {
    Add (code);
  }
} // namespace s1
