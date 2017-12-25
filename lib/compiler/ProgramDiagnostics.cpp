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
#include "base/ResultCode_internal.h"

#include "compiler/ProgramDiagnostics.h"

#include "base/MultiOptional.h"
#include "diagnostics/common.h"
#include "lexer/Lexer.h"

namespace s1
{
  struct Compiler::ProgramDiagnostics::Entry
  {
    unsigned int code;
    s1::MultiOptional<uc::String, uc::String> info;
  };

  Compiler::ProgramDiagnostics::ProgramDiagnostics (s1::Library* lib)
    : LibraryObject (lib)
  {}

  Compiler::ProgramDiagnostics::~ProgramDiagnostics () {}

  size_t Compiler::ProgramDiagnostics::GetCount() const
  {
    return entries.size();
  }

  Result<Compiler::ProgramDiagnostics::Class> Compiler::ProgramDiagnostics::Classify (size_t index) const
  {
    if (index >= entries.size ()) return S1_E_INVALID_ARG_N (0);
    return diagnostics::IsWarning (entries[index].code) ? Class::Warning : Class::Error;
  }

  Result<const char*> Compiler::ProgramDiagnostics::Id (size_t index) const
  {
    if (index >= entries.size ()) return S1_E_INVALID_ARG_N (0);
    auto id_str = diagnostics::GetIdString (entries[index].code);
    S1_ASSERT_MSG (id_str, "A diagnostic code is invalid", nullptr);
    return id_str;
  }

  Result<uc::String> Compiler::ProgramDiagnostics::Description (size_t index) const
  {
    if (index >= entries.size ()) return S1_E_INVALID_ARG_N (0);
    auto descr_str = diagnostics::GetDescriptionString (entries[index].code);
    S1_ASSERT_MSG (descr_str, "A diagnostic code is invalid", uc::String ());
    return descr_str;
  }

  void Compiler::ProgramDiagnostics::Add (unsigned int code,
                                          boost::optional<const uc::String&> info1,
                                          boost::optional<const uc::String&> info2)
  {
    Entry newEntry;
    newEntry.code = code;
    if (info1) newEntry.info.emplace<0> (*info1);
    if (info2) newEntry.info.emplace<1> (*info2);
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
