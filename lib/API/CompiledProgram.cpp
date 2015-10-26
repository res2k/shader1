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

#include "s1/CompiledProgram.h"

#include "base/common.h"

#include "compiler/Backend.h"

const char* s1_compiledprogram_get_string (s1_CompiledProgram* program)
{
  S1_ASSERT_MSG(program, "NULL CompiledProgram", nullptr);
  s1::Compiler::Backend::Program* program_impl (s1::EvilUpcast<s1::Compiler::Backend::Program> (program));

  return program_impl->Return (program_impl->Try<const char*> ([=]() {
      return program_impl->GetProgramString().c_str();
    }), nullptr);
}
