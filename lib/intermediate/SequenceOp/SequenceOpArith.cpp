#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpArith.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpArith::SequenceOpArith (RegisterPtr destination,
				      SequenceVisitor::ArithmeticOp op,
				      RegisterPtr source1,
				      RegisterPtr source2)
     : SequenceOpBinary (destination, source1, source2), op (op)
    {
    }
    
    void SequenceOpArith::Visit (SequenceVisitor& visitor)
    {
      visitor.OpArith (destination, op, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
