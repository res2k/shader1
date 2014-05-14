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

#ifndef __COMPILER_BACKEND_H__
#define __COMPILER_BACKEND_H__

#include "base/LibraryObject.h"

#include "Compiler.h"

#include "intermediate/forwarddecl.h"
#include "splitter/Frequency.h"

namespace s1
{
  class Compiler::Backend : public LibraryObject
  {
  public:
    class Program : public LibraryObject
    {
    public:
      Program (Library* lib) : LibraryObject (lib) {}

      virtual std::string GetProgramString () = 0;
    };
    typedef boost::intrusive_ptr<Program> ProgramPtr;
    
    Backend (Library* lib) : LibraryObject (lib) {}
    
    enum CompileTarget
    {
      targetUnsplit,
      targetVP,
      targetFP
    };
    virtual ProgramPtr GenerateProgram (CompileTarget target,
					const intermediate::ProgramPtr& prog) = 0;
  };
} // namespace s1

#endif // __COMPILER_BACKEND_H__
