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
      for (intermediate::Program::OutputParameters::const_iterator outputParam = program->GetOutputParameters().begin();
	   outputParam != program->GetOutputParameters().end();
	   ++outputParam)
      {
	newProgram->SetOutputParameter (outputParam->first, outputParam->second);
      }      
      {
	const intermediate::Program::ParameterArraySizes& progArraySizes = program->GetParameterArraySizes ();
	for (intermediate::Program::ParameterArraySizes::const_iterator paramArraySize = progArraySizes.begin();
	     paramArraySize != progArraySizes.end();
	     ++paramArraySize)
	{
	  newProgram->SetParameterArraySize (paramArraySize->first, paramArraySize->second);
	}
      }
      
      for (size_t f = 0; f < program->GetNumFunctions(); f++)
      {
	intermediate::ProgramFunctionPtr func (program->GetFunction (f));
	
	intermediate::RegisterSet usedRegs;
	const intermediate::Sequence::RegisterExpMappings seqExports (func->GetBody()->GetExports ());
	// Collect function outputs, mark them as 'used' for DCE
	BOOST_FOREACH(const parser::SemanticsHandler::Scope::FunctionFormalParameter& param, func->GetParams())
	{
	  if ((param.dir & parser::SemanticsHandler::Scope::dirOut) != 0)
	  {
	    const intermediate::Sequence::RegisterExpMappings::const_iterator exp = seqExports.find (param.identifier);
	    if (exp == seqExports.end()) continue;
	    usedRegs.insert (exp->second);	  
	  }
	}
	// Also, all transferred regs must be considered 'used'
	BOOST_FOREACH(const intermediate::ProgramFunction::TransferMappingPair& tmp, func->GetTransferMappings())
	{
	  usedRegs.insert (tmp.second);
	}
	
	intermediate::SequencePtr newBody;
	SequenceOptimizations::Apply (optimizations, newBody, func->GetBody(), usedRegs);
	
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
