#ifndef __SEQUENCEOPTIMIZATIONS_H__
#define __SEQUENCEOPTIMIZATIONS_H__

#include "intermediate/forwarddecl.h"
#include "intermediate/Sequence.h"

#include <vector>

namespace s1
{
  namespace optimize
  {
    class SequenceOptimizations
    {
    public:
      static bool Apply (unsigned int optimizations,
			 intermediate::SequencePtr& outputSeq,
			 const intermediate::SequencePtr& inputSeq,
			 const intermediate::RegisterSet& usedRegs);
    };
  } // namespace optimize
} // namespace s1

#endif // __SEQUENCEOPTIMIZATIONS_H__
