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
#include "SequenceOptimizations.h"
#include "OptimizeSequenceStep.h"
#include "OptimizeSequenceConstantFolding.h"
#include "OptimizeSequenceDeadCodeEliminate.h"
#include "OptimizeSequenceInlineBlocks.h"

#include "intermediate/Sequence.h"
#include "intermediate/SequenceBuilder.h"
#include "optimize/Optimizer.h"

#include <boost/make_shared.hpp>
#include <vector>

namespace s1
{
  namespace optimize
  {
    typedef boost::shared_ptr<OptimizeSequenceStep> OptimizeSequenceStepPtr;
    
    bool SequenceOptimizations::Apply (unsigned int optimizations,
				       intermediate::SequencePtr& outputSeq,
				       const intermediate::SequencePtr& inputSeq,
				       const intermediate::RegisterSet& usedRegs)
    {
      bool seqChanged = false;
      intermediate::SequencePtr currentSeq (inputSeq);
      
      unsigned int currentOpt = optimizations;
      while (currentOpt != 0)
      {
	unsigned int newOpt = 0;
	std::vector<OptimizeSequenceStepPtr> steps;
	if (currentOpt & Optimizer::optInlineBlocks)
	{
	  steps.push_back (boost::make_shared<OptimizeSequenceInlineBlocks> ());
	}
	if (currentOpt & Optimizer::optConstantFolding)
	{
	  steps.push_back (boost::make_shared<OptimizeSequenceConstantFolding> ());
	}
	if (currentOpt & Optimizer::optDeadCodeElimination)
	{
	  steps.push_back (boost::make_shared<OptimizeSequenceDeadCodeEliminate> (usedRegs));
	}
	
	for(const OptimizeSequenceStepPtr& step : steps)
	{
	  intermediate::SequenceBuilderPtr newSeqBuilder (boost::make_shared<intermediate::SequenceBuilder> ());
	  newSeqBuilder->AddImports (currentSeq->GetImports ());
	  newSeqBuilder->AddExports (currentSeq->GetExports ());
	  //newSeqBuilder->SetIdentifierRegisters  (currentSeq->GetIdentifierToRegisterMap());
	  unsigned int changes = step->Apply (newSeqBuilder, currentSeq);
	  currentSeq = newSeqBuilder->GetSequence();
	  newOpt = step->FilterOptimizerFlags (newOpt);
	  
	  if (changes & OptimizeSequenceStep::opsExpanded)
	  {
	    newOpt |= Optimizer::optDeadCodeElimination | Optimizer::optConstantFolding;
	  }
	  if (changes & OptimizeSequenceStep::opsRemoved)
	  {
	  }
	  if (changes & OptimizeSequenceStep::opsBlocksChanged)
	  {
	    newOpt |= Optimizer::optInlineBlocks | Optimizer::optDeadCodeElimination;
	  }
	}
	
	currentOpt = newOpt & optimizations;
      }
      outputSeq = currentSeq;
      return seqChanged;      
    }
  } // namespace optimize
} // namespace s1
