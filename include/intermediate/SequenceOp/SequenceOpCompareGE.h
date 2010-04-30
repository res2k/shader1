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
      SequenceOpCompareGE (RegisterID destination,
			   RegisterID source1,
			   RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCECOMPAREGE_H__
