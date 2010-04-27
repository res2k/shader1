#ifndef __INTERMEDIATE_SEQUENCECOMPARELE_H__
#define __INTERMEDIATE_SEQUENCECOMPARELE_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpCompareLE : public SequenceOpBinary
    {
    public:
      SequenceOpCompareLE (Sequence::RegisterID destination,
			   Sequence::RegisterID source1,
			   Sequence::RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCECOMPARELE_H__
