#include "base/common.h"
#include "base/hash_UnicodeString.h"

#include "splitter/ProgramSplitter.h"

#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"
#include "splitter/SequenceSplitter.h"

#include <boost/make_shared.hpp>

namespace s1
{
  namespace splitter
  {
    void ProgramSplitter::SetInputProgram (const intermediate::ProgramPtr& program)
    {
      inputProgram = program;
    }

    void ProgramSplitter::SetInputFreqFlags (const UnicodeString& inpName, unsigned int flags)
    {
      paramFlags[inpName] = flags;
    }
    
    void ProgramSplitter::PerformSplit ()
    {
      outputProgram = boost::make_shared<intermediate::Program> ();
  
      // TODO: Handle param flags
  
      for (size_t i = 0; i < inputProgram->GetNumFunctions(); i++)
      {
	intermediate::ProgramFunctionPtr func = inputProgram->GetFunction (i);
	if (!func->IsEntryFunction()) continue;
	
	splitter::SequenceSplitter splitter;
	splitter.SetInputSequence (func->GetBody());
	for (ParamMap::const_iterator paramFlag = paramFlags.begin();
	     paramFlag != paramFlags.end();
	     ++paramFlag)
	{
	  splitter.SetInputFreqFlags (paramFlag->first, paramFlag->second);
	}
	splitter.PerformSplit();
	
	UnicodeString funcVName ("vertex_");
	funcVName.append (func->GetIdentifier());
	
	intermediate::ProgramFunctionPtr funcV (boost::make_shared<intermediate::ProgramFunction> (funcVName,
												   func->GetReturnType(),
												   func->GetParams(),
												   splitter.GetOutputVertexSequence(),
												   func->IsEntryFunction()));
	outputProgram->AddFunction (funcV);

	UnicodeString funcFName ("fragment_");
	funcFName.append (func->GetIdentifier());
	
	intermediate::ProgramFunctionPtr funcF (boost::make_shared<intermediate::ProgramFunction> (funcFName,
												   func->GetReturnType(),
												   func->GetParams(),
												   splitter.GetOutputFragmentSequence(),
												   func->IsEntryFunction()));
	outputProgram->AddFunction (funcF);
      }
    }
    
    const intermediate::ProgramPtr& ProgramSplitter::GetOutputProgram ()
    {
      return outputProgram;
    }
    
  } // namespace splitter
} // namespace s1
