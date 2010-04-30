#ifndef __INTERMEDIATE_SEQUENCECOMPARENE_H__
#define __INTERMEDIATE_SEQUENCECOMPARENE_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpCompareNE : public SequenceOpBinary
    {
    public:
      SequenceOpCompareNE (RegisterID destination,
			   RegisterID source1,
			   RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCECOMPARENE_H__
