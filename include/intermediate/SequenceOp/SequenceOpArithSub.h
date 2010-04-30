#ifndef __INTERMEDIATE_SEQUENCEOPARITHSUB_H__
#define __INTERMEDIATE_SEQUENCEOPARITHSUB_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpArithSub : public SequenceOpBinary
    {
    public:
      SequenceOpArithSub (RegisterID destination,
			  RegisterID source1,
			  RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPARITHSUB_H__
