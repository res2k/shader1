#include "intermediate/SequenceOp/SequenceOpArith.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpArith::SequenceOpArith (RegisterID destination,
				      SequenceVisitor::ArithmeticOp op,
				      RegisterID source1,
				      RegisterID source2)
     : SequenceOpBinary (destination, source1, source2), op (op)
    {
    }
    
    void SequenceOpArith::Visit (SequenceVisitor& visitor)
    {
      visitor.OpArith (destination, op, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
