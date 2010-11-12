#ifndef __INTERMEDIATE_SEQUENCECOMPARE_H__
#define __INTERMEDIATE_SEQUENCECOMPARE_H__

#include "../SequenceVisitor.h"
#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpCompare : public SequenceOpBinary
    {
      SequenceVisitor::CompareOp op;
    public:
      SequenceOpCompare (RegisterPtr destination,
			 SequenceVisitor::CompareOp op,
			 RegisterPtr source1,
			 RegisterPtr source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCECOMPARE_H__
