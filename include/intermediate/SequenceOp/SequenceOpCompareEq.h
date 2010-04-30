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
      SequenceOpCompareEq (RegisterID destination,
			   RegisterID source1,
			   RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCECOMPAREEQ_H__
