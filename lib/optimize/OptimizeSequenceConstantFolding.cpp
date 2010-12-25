#include "base/common.h"
#include "OptimizeSequenceConstantFolding.h"

#include "optimize/ConstantFolding.h"

namespace s1
{
  namespace optimize
  {
    unsigned int OptimizeSequenceConstantFolding::Apply (const intermediate::SequencePtr& outputSeq,
							 const intermediate::SequencePtr& inputSeq)
    {
      bool seqChanged = ConstantFolding::FoldConstants (outputSeq, inputSeq);
      
      if (seqChanged)
      {
	return opsRemoved;
      }
      else
	return 0;
    }
  } // namespace optimize
} // namespace s1

