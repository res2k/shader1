#ifndef __DEADCODEELIMINATION_H__
#define __DEADCODEELIMINATION_H__

#include "intermediate/Sequence.h"

namespace s1
{
  namespace optimize
  {
    class DeadCodeElimination
    {
      class DeadCodeChecker;
    public:
      static bool EliminateDeadCode (const intermediate::SequencePtr& outputSeq,
				     const intermediate::SequencePtr& inputSeq,
				     const intermediate::RegisterSet& usedRegistersSeed);
    };
  } // namespace optimize
} // namespace s1

#endif // __DEADCODEELIMINATION_H__
