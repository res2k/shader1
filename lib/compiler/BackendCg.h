#ifndef __COMPILER_BACKENDCG_H__
#define __COMPILER_BACKENDCG_H__

#include "compiler/Backend.h"

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
	std::string GetProgramString ();
      };
    public:
      BackendCg (Library* lib);
    
      ProgramPtr GenerateProgram (CompileTarget target,
				  const intermediate::ProgramPtr& prog);
    };
  } // namespace compiler
} // namespace s1

#endif // __COMPILER_BACKENDCG_H__
