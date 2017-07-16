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

#ifndef __COMPILER_BACKENDPROGRAMIMPL_H__
#define __COMPILER_BACKENDPROGRAMIMPL_H__

#include "BackendProgram.h"

namespace s1
{
  /// Default program implementation (simply returning strings)
  class Compiler::Backend::ProgramImpl : public Program
  {
    codegen::StringsArrayPtr programLines;
  public:
    ProgramImpl (const codegen::StringsArrayPtr& programLines)
      : programLines (programLines) {}

    const codegen::StringsArrayPtr& GetProgramLines () override
    {
      return programLines;
    }
  };
} // namespace s1

#endif // __COMPILER_BACKENDPROGRAMIMPL_H__
