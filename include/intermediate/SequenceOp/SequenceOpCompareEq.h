#ifndef __INTERMEDIATE_SEQUENCECOMPAREEQ_H__
#define __INTERMEDIATE_SEQUENCECOMPAREEQ_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpCompareEq : public SequenceOpBinary
    {
    public:
      SequenceOpCompareEq (Sequence::RegisterID destination,
			   Sequence::RegisterID source1,
			   Sequence::RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCECOMPAREEQ_H__
