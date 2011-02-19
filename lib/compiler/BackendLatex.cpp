#include "base/common.h"

#include "BackendLatex.h"

#include "codegen/LatexGenerator.h"
#include "DeprotectCtor.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace compiler
  {
    BackendLatex::BackendLatex()
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
    
    Compiler::Backend::ProgramPtr BackendLatex::GenerateProgram (CompileTarget target,
								 const intermediate::ProgramPtr& prog)
    {
      codegen::LatexGenerator codegen;
      codegen::StringsArrayPtr outputProg (codegen.Generate (prog));
      
      return boost::make_shared<DeprotectCtor<LatexProgram> > (FlattenStringArray (outputProg));
    }
    
    //-----------------------------------------------------------------------
    
    std::string BackendLatex::LatexProgram::GetProgramString ()
    {
      return programString;
    }

  } // namespace compiler
} // namespace s1
