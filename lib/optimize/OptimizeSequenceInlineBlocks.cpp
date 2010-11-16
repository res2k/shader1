#include "OptimizeSequenceInlineBlocks.h"

#include "optimize/Inliner.h"

namespace s1
{
  namespace optimize
  {
    unsigned int OptimizeSequenceInlineBlocks::Apply (const intermediate::SequencePtr& outputSeq,
						      const intermediate::SequencePtr& inputSeq)
    {
      bool seqChanged = Inliner::InlineAllBlocks (outputSeq, inputSeq);
      
      return seqChanged ? opsExpanded : 0;
    }
  } // namespace optimize
} // namespace s1

