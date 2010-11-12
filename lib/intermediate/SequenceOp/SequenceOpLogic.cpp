#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpLogic.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpLogic::SequenceOpLogic (RegisterPtr destination,
				      SequenceVisitor::LogicOp op,
				      RegisterPtr source1,
				      RegisterPtr source2)
     : SequenceOpBinary (destination, source1, source2), op (op)
    {
    }
    
    void SequenceOpLogic::Visit (SequenceVisitor& visitor)
    {
      visitor.OpLogic (destination, op, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
