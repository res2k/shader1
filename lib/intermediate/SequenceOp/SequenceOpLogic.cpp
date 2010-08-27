#include "intermediate/SequenceOp/SequenceOpLogic.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpLogic::SequenceOpLogic (RegisterID destination,
				      SequenceVisitor::LogicOp op,
				      RegisterID source1,
				      RegisterID source2)
     : SequenceOpBinary (destination, source1, source2), op (op)
    {
    }
    
    void SequenceOpLogic::Visit (SequenceVisitor& visitor)
    {
      visitor.OpLogic (destination, op, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
