#ifndef __OPTIMIZE_OPTIMIZESEQUENCESTEP_H__
#define __OPTIMIZE_OPTIMIZESEQUENCESTEP_H__

#include "intermediate/forwarddecl.h"

namespace s1
{
  namespace optimize
  {
    struct OptimizeSequenceStep
    {
      virtual ~OptimizeSequenceStep() {}
      
      /// Flags indicating how the optimize step changed the sequence
      enum
      {
	/**
	 * Some operations were expanded into multiple operations.
	 * Might give new opportunities for dead code removal or
	 * constant propagation.
	 */
	opsExpanded = 1 << 0
      };
      virtual unsigned int Apply (const intermediate::SequencePtr& outputSeq,
				  const intermediate::SequencePtr& inputSeq) = 0;
    };
  } // namespace optimize
} // namespace s1

#endif // __OPTIMIZE_OPTIMIZESEQUENCESTEP_H__
