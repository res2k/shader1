#ifndef __OPTIMIZE_OPTIMIZESEQUENCEDEADCODEELIMINATE_H__
#define __OPTIMIZE_OPTIMIZESEQUENCEDEADCODEELIMINATE_H__

#include "OptimizeSequenceStep.h"

#include "intermediate/Sequence.h"
#include "optimize/Optimizer.h"

namespace s1
{
  namespace optimize
  {
    class OptimizeSequenceDeadCodeEliminate : public OptimizeSequenceStep
    {
      intermediate::RegisterSet usedRegistersSeed;
    public:
      OptimizeSequenceDeadCodeEliminate (const intermediate::RegisterSet& usedRegistersSeed);
      
      unsigned int Apply (const intermediate::SequencePtr& outputSeq,
			  const intermediate::SequencePtr& inputSeq);
			  
      unsigned int FilterOptimizerFlags (unsigned int optimizers)
      { return optimizers & ~Optimizer::optDeadCodeElimination; }
    };
  } // namespace optimize
} // namespace s1

#endif // __OPTIMIZE_OPTIMIZESEQUENCEDEADCODEELIMINATE_H__
