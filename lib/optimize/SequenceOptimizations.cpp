#include "base/common.h"
#include "SequenceOptimizations.h"
#include "OptimizeSequenceStep.h"
#include "OptimizeSequenceConstantFolding.h"
#include "OptimizeSequenceDeadCodeEliminate.h"
#include "OptimizeSequenceInlineBlocks.h"

#include "intermediate/Sequence.h"
#include "optimize/Optimizer.h"

#include <boost/foreach.hpp>
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
	
	BOOST_FOREACH(const OptimizeSequenceStepPtr& step, steps)
	{
	  intermediate::SequencePtr newSeq (boost::make_shared<intermediate::Sequence> ());
	  newSeq->AddImports (currentSeq->GetImports ());
	  newSeq->AddExports (currentSeq->GetExports ());
	  newSeq->SetIdentifierRegisters  (currentSeq->GetIdentifierToRegisterMap());
	  unsigned int changes = step->Apply (newSeq, currentSeq);
	  currentSeq = newSeq;
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
