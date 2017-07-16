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

#include "base/common.h"
#include "base/ResultCode_internal.h"

#include "CompiledProgram.h"

#include "compiler/Backend.h"
#include "compiler/BackendProgram.h"

/// Join the strings arrays with newlines.
static std::string FlattenStringArray (const s1::codegen::StringsArrayPtr& strings)
{
  std::string str;
  for (size_t i = 0; i < strings->Size(); i++)
  {
    str.append (strings->Get (i));
    str.append ("\n");
  }
  return str;
}

namespace s1
{
  namespace api_impl
  {
    CompiledProgram::CompiledProgram (s1::Library* lib, Compiler::Backend::ProgramPtr compiledProgram)
      : LibraryObject (lib), compiledProgram (compiledProgram)
    {}

    const char* CompiledProgram::GetString ()
    {
      if (!flatString)
      {
        flatString = FlattenStringArray (compiledProgram->GetProgramLines());
        compiledProgram->DiscardLines ();
      }
      return flatString->c_str();
    }
  } // namespace api_impl
} // namespace s1

const char* s1_compiledprogram_get_string (s1_CompiledProgram* program)
{
  S1_ASSERT_MSG(program, "NULL CompiledProgram", nullptr);
  auto program_impl (s1::EvilUpcast<s1::api_impl::CompiledProgram> (program));
  s1::ScopedThreadDebugMessageHandler setMsgHandler (program_impl->GetDebugMessageHandler ());

  return program_impl->Return (program_impl->Try ([=]() {
      return program_impl->GetString();
    }), nullptr);
}
