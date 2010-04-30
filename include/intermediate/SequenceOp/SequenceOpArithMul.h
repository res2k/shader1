#ifndef __INTERMEDIATE_SEQUENCEOPARITHMUL_H__
#define __INTERMEDIATE_SEQUENCEOPARITHMUL_H__

#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpArithMul : public SequenceOpBinary
    {
    public:
      SequenceOpArithMul (RegisterID destination,
			  RegisterID source1,
			  RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPARITHMUL_H__
