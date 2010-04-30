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
      SequenceOpCompareLE (RegisterID destination,
			   RegisterID source1,
			   RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCECOMPARELE_H__
