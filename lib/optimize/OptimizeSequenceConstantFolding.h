#ifndef __OPTIMIZE_OPTIMIZESEQUENCECONSTANTFOLDING_H__
#define __OPTIMIZE_OPTIMIZESEQUENCECONSTANTFOLDING_H__

#include "optimize/Optimizer.h"
#include "OptimizeSequenceStep.h"

namespace s1
{
  namespace optimize
  {
    class OptimizeSequenceConstantFolding : public OptimizeSequenceStep
    {
    public:
      unsigned int Apply (const intermediate::SequencePtr& outputSeq,
			  const intermediate::SequencePtr& inputSeq);
			  
      unsigned int FilterOptimizerFlags (unsigned int optimizers)
      { return optimizers & ~Optimizer::optConstantFolding; }
    };
  } // namespace optimize
} // namespace s1

#endif // __OPTIMIZE_OPTIMIZESEQUENCECONSTANTFOLDING_H__
