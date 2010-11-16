#ifndef __OPTIMIZE_OPTIMIZESEQUENCEINLINEBLOCKS_H__
#define __OPTIMIZE_OPTIMIZESEQUENCEINLINEBLOCKS_H__

#include "OptimizeSequenceStep.h"

namespace s1
{
  namespace optimize
  {
    class OptimizeSequenceInlineBlocks : public OptimizeSequenceStep
    {
    public:
      unsigned int Apply (const intermediate::SequencePtr& outputSeq,
			  const intermediate::SequencePtr& inputSeq);
    };
  } // namespace optimize
} // namespace s1

#endif // __OPTIMIZE_OPTIMIZESEQUENCESTEP_H__
