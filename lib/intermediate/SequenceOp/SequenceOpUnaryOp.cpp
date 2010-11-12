#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpUnaryOp.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpUnaryOp::SequenceOpUnaryOp (RegisterPtr destination,
					  SequenceVisitor::UnaryOp op,
					  RegisterPtr source)
     : SequenceOpUnary (destination, source), op (op)
    {
    }
    
    void SequenceOpUnaryOp::Visit (SequenceVisitor& visitor)
    {
      visitor.OpUnary (destination, op, source);
    }
  } // namespace intermediate
} // namespace s1
