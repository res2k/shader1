#include "intermediate/SequenceOp/SequenceOpUnaryOp.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpUnaryOp::SequenceOpUnaryOp (RegisterID destination,
					  SequenceVisitor::UnaryOp op,
					  RegisterID source)
     : SequenceOpUnary (destination, source), op (op)
    {
    }
    
    void SequenceOpUnaryOp::Visit (SequenceVisitor& visitor)
    {
      visitor.OpUnary (destination, op, source);
    }
  } // namespace intermediate
} // namespace s1
