#ifndef __INTERMEDIATE_SEQUENCEOPARITH_H__
#define __INTERMEDIATE_SEQUENCEOPARITH_H__

#include "../SequenceVisitor.h"
#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpArith : public SequenceOpBinary
    {
      SequenceVisitor::ArithmeticOp op;
    public:
      SequenceOpArith (RegisterID destination,
		       SequenceVisitor::ArithmeticOp op,
		       RegisterID source1,
		       RegisterID source2);
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPARITH_H__
