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

#include "base/format/Formatter.h"
#include "base/format/uc_String.h"
#include "base/MultiOptional.h"
#include "diagnostics/common.h"
#include "lexer/Lexer.h"

#include "base/format/Formatter.ipp"

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
    const auto& entry = entries[index];
    auto descr_str = diagnostics::GetDescriptionString (entry.code);
    uc::String empty_str;
    S1_ASSERT_MSG (descr_str, "A diagnostic code is invalid", empty_str);
    format::Formatter formatDescription (descr_str);
    return formatDescription.to<uc::String> (entry.info.value_or<0> (empty_str),
                                             entry.info.value_or<1> (empty_str));
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

  boost::optional<uc::String> Compiler::ProgramDiagnostics::ConvertErrorInfo (const ErrorInfoType& info)
  {
    class InfoVisitor : public boost::static_visitor<boost::optional<uc::String>>
    {
    public:
      boost::optional<uc::String> operator()(nullopt_t) const
      {
        return boost::none;
      }

      boost::optional<uc::String> operator()(const uc::String& value) const
      {
        return value;
      }

      boost::optional<uc::String> operator()(const lexer::Token& value) const
      {
        return Lexer::GetTokenStr (value);
      }

      boost::optional<uc::String> operator()(const lexer::TokenType& value) const
      {
        return uc::String (Lexer::GetTokenStr (value));
      }
    };
    InfoVisitor visitor;

    return info.apply_visitor (visitor);
  }

  void Compiler::ProgramDiagnostics::LexerErrorImpl (unsigned int code, const lexer::TokenLocation& location)
  {
    // TODO: Actually use location
    Add (code);
  }

  template<typename T>
  static boost::optional<const T&> cref (const boost::optional<T>& x)
  {
    if (x)
      return *x;
    else
      return boost::none;
  }

  void Compiler::ProgramDiagnostics::ParseErrorImpl (unsigned int code,
                                                     ErrorInfoType info1,
                                                     ErrorInfoType info2)
  {
    const auto info1str = ConvertErrorInfo (info1);
    const auto info2str = ConvertErrorInfo (info2);
    Add (code, cref (info1str), cref (info2str));
  }

  void Compiler::ProgramDiagnostics::SemanticErrorImpl (unsigned int code)
  {
    Add (code);
  }
} // namespace s1
