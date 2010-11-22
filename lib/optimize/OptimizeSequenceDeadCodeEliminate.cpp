#include "base/common.h"
#include "OptimizeSequenceDeadCodeEliminate.h"

#include "optimize/DeadCodeElimination.h"

namespace s1
{
  namespace optimize
  {
    OptimizeSequenceDeadCodeEliminate::OptimizeSequenceDeadCodeEliminate (const intermediate::RegisterSet& usedRegistersSeed)
     : usedRegistersSeed (usedRegistersSeed)
    {
    }
    
    unsigned int OptimizeSequenceDeadCodeEliminate::Apply (const intermediate::SequencePtr& outputSeq,
							   const intermediate::SequencePtr& inputSeq)
    {
      bool seqChanged = DeadCodeElimination::EliminateDeadCode (outputSeq, inputSeq, usedRegistersSeed);
      
      if (seqChanged)
      {
	outputSeq->CleanUnusedImportsExports();
	return opsRemoved;
      }
      else
	return 0;
    }
  } // namespace optimize
} // namespace s1

