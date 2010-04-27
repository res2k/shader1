#ifndef __INTERMEDIATE_SEQUENCECOMPAREGE_H__
#define __INTERMEDIATE_SEQUENCECOMPAREGE_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpCompareGE : public SequenceOpBinary
    {
    public:
      SequenceOpCompareGE (Sequence::RegisterID destination,
			   Sequence::RegisterID source1,
			   Sequence::RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCECOMPAREGE_H__
