#ifndef __INTERMEDIATE_SEQUENCECOMPAREGT_H__
#define __INTERMEDIATE_SEQUENCECOMPAREGT_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpCompareGT : public SequenceOpBinary
    {
    public:
      SequenceOpCompareGT (RegisterID destination,
			   RegisterID source1,
			   RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCECOMPAREGT_H__
