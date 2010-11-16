#include "base/common.h"
#include "SequenceOptimizations.h"
#include "OptimizeSequenceStep.h"
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
				       const intermediate::SequencePtr& inputSeq)
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
	
	BOOST_FOREACH(const OptimizeSequenceStepPtr& step, steps)
	{
	  intermediate::SequencePtr newSeq (boost::make_shared<intermediate::Sequence> ());
	  unsigned int changes = step->Apply (newSeq, currentSeq);
	  currentSeq = newSeq;
	  
	  if (changes & OptimizeSequenceStep::opsExpanded)
	  {
	    // newOpt |= ...
	  }
	}
	
	currentOpt = newOpt & optimizations;
      }
      outputSeq = currentSeq;
      return seqChanged;      
    }
  } // namespace optimize
} // namespace s1
