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

#include "s1/ProgramDiagnostics.h"

#include "compiler/ProgramDiagnostics.h"

size_t s1_programdiagnostics_get_count (s1_ProgramDiagnostics* diagnostics)
{
  S1_ASSERT_MSG(diagnostics, "NULL ProgramDiagnostics", 0);
  s1::Compiler::ProgramDiagnostics* diagnostics_impl (
    s1::EvilUpcast<s1::Compiler::ProgramDiagnostics> (diagnostics));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (diagnostics_impl->GetDebugMessageHandler ());

  return diagnostics_impl->Return (diagnostics_impl->Try (
    [=]()
    {
      return diagnostics_impl->GetCount();
    }), 0);
}

enum s1_DiagnosticClass s1_programdiagnostics_get_class (
  s1_ProgramDiagnostics* diagnostics, size_t diagnosticIndex)
{
  S1_ASSERT_MSG(diagnostics, "NULL ProgramDiagnostics", S1_DIAGNOSTIC_INVALID);
  s1::Compiler::ProgramDiagnostics* diagnostics_impl (
    s1::EvilUpcast<s1::Compiler::ProgramDiagnostics> (diagnostics));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (diagnostics_impl->GetDebugMessageHandler ());

  return diagnostics_impl->Return (diagnostics_impl->Try (
    [=]() -> s1::Result<s1_DiagnosticClass>
    {
      return diagnostics_impl->Classify (diagnosticIndex).filter (
        [=](s1::Compiler::ProgramDiagnostics::Class value)
        {
          switch (value)
          {
          case s1::Compiler::ProgramDiagnostics::Class::Error:
            return S1_DIAGNOSTIC_ERROR;
          case s1::Compiler::ProgramDiagnostics::Class::Warning:
            return S1_DIAGNOSTIC_WARNING;
          }
          return S1_DIAGNOSTIC_INVALID;
        }
      );
    }), S1_DIAGNOSTIC_INVALID);
}

const char* s1_programdiagnostics_get_id (s1_ProgramDiagnostics* diagnostics,
  size_t diagnosticIndex)
{
  S1_ASSERT_MSG(diagnostics, "NULL ProgramDiagnostics", nullptr);
  s1::Compiler::ProgramDiagnostics* diagnostics_impl (
    s1::EvilUpcast<s1::Compiler::ProgramDiagnostics> (diagnostics));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (diagnostics_impl->GetDebugMessageHandler ());

  return diagnostics_impl->Return (diagnostics_impl->Try (
    [=]()
    {
      return diagnostics_impl->Id (diagnosticIndex);
    }), nullptr);
}

