#ifndef __COMPILER_BACKENDLATEX_H__
#define __COMPILER_BACKENDLATEX_H__

#include "compiler/Backend.h"

namespace s1
{
  namespace compiler
  {
    class BackendLatex : public Compiler::Backend
    {
      class LatexProgram : public Program
      {
	std::string programString;
      protected:
	friend class BackendLatex;
	LatexProgram (const std::string& programString) : programString (programString) {}
      public:
	std::string GetProgramString ();
      };
    public:
      BackendLatex ();
    
      ProgramPtr GenerateProgram (CompileTarget target,
				  const intermediate::ProgramPtr& prog);
    };
  } // namespace compiler
} // namespace s1

#endif // __COMPILER_BACKENDLATEX_H__
