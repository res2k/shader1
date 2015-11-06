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

#ifndef __COMPILER_BACKENDCG_H__
#define __COMPILER_BACKENDCG_H__

#include "compiler/Backend.h"
#include "codegen/cg/CgOptions.h"

namespace s1
{
  namespace compiler
  {
    class BackendCg : public Compiler::Backend
    {
      class CgProgram : public Program
      {
	std::string programString;
      protected:
        friend class BackendCg;
	CgProgram (Library* lib, const std::string& programString)
          : Program (lib), programString (programString) {}
      public:
	const std::string& GetProgramString ();
      };
      class CgOptions : public OptionsImpl<codegen::CgOptions>
      {
      protected:
        friend class BackendCg;
        CgOptions (Library* lib) : OptionsImpl (lib) {}
      };
    public:
      BackendCg (Library* lib);
    
      OptionsPtr CreateOptions ();
      ProgramPtr GenerateProgram (CompileTarget target,
				  const intermediate::ProgramPtr& prog,
                                  OptionsPtr options);
    };
  } // namespace compiler
} // namespace s1

#endif // __COMPILER_BACKENDCG_H__
