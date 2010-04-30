#ifndef __INTERMEDIATE_SEQUENCECOMPARELT_H__
#define __INTERMEDIATE_SEQUENCECOMPARELT_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpCompareLT : public SequenceOpBinary
    {
    public:
      SequenceOpCompareLT (RegisterID destination,
			   RegisterID source1,
			   RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCECOMPARELT_H__
