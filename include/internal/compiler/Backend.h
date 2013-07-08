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
