#include "base/common.h"

#include "BackendCg.h"

#include "codegen/CgGenerator.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace compiler
  {
    BackendCg::BackendCg (Library* lib) : Backend (lib)
    {
    }
    
    static std::string FlattenStringArray (const codegen::StringsArrayPtr& strings)
    {
      std::string str;
      for (size_t i = 0; i < strings->Size(); i++)
      {
	str.append (strings->Get (i));
	str.append ("\n");
      }
      return str;
    }
    
    Compiler::Backend::ProgramPtr BackendCg::GenerateProgram (CompileTarget target,
							      const intermediate::ProgramPtr& prog)
    {
      int freq = -1;
      switch (target)
      {
      case targetVP:	freq = splitter::freqVertex; 	break;
      case targetFP:	freq = splitter::freqFragment; 	break;
      default:						break;
      }
      
      codegen::CgGenerator codegen;
      codegen::StringsArrayPtr outputProg (codegen.Generate (prog, freq));
      
      return ProgramPtr (new CgProgram (GetLibrary(), FlattenStringArray (outputProg)));
    }
    
    //-----------------------------------------------------------------------
    
    std::string BackendCg::CgProgram::GetProgramString ()
    {
      return programString;
    }

  } // namespace compiler
} // namespace s1
