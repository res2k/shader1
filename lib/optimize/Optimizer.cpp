#include "base/common.h"
#include "optimize/Optimizer.h"
#include "SequenceOptimizations.h"

#include "intermediate/Program.h"
#include "intermediate/ProgramFunction.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

namespace s1
{
  namespace optimize
  {
    intermediate::ProgramPtr Optimizer::ApplyOptimizations (const intermediate::ProgramPtr& program)
    {
      intermediate::ProgramPtr newProgram (boost::make_shared<intermediate::Program> ());
      BOOST_FOREACH(const intermediate::Program::TransferValuePair& tvp, program->GetTransferValues())
      {
	newProgram->AddTransferValue (tvp.first, tvp.second);
      }
      
      for (size_t f = 0; f < program->GetNumFunctions(); f++)
      {
	intermediate::ProgramFunctionPtr func (program->GetFunction (f));
	
	intermediate::SequencePtr newBody;
	SequenceOptimizations::Apply (optimizations, newBody, func->GetBody());
	
	intermediate::ProgramFunctionPtr newFunc (
	  boost::make_shared<intermediate::ProgramFunction> (func->GetOriginalIdentifier(),
							     func->GetIdentifier(),
							     func->GetParams(),
							     newBody,
							     func->IsEntryFunction()));
							     
	BOOST_FOREACH(const intermediate::ProgramFunction::TransferMappingPair& tmp, func->GetTransferMappings())
	{
	  newFunc->SetTransferMapping (tmp.first, tmp.second);
	}
							     
	for (intermediate::ProgramFunction::ParameterFrequencyMap::const_iterator paramFreq = func->GetParameterFrequencies().begin();
	     paramFreq != func->GetParameterFrequencies().end();
	     ++paramFreq)
	{
	  newFunc->SetParameterFrequency (paramFreq->first, paramFreq->second);
	}
							     
	newProgram->AddFunction (newFunc);
      }
      
      return newProgram;
    }
  } // namespace optimize
} // namespace s1
