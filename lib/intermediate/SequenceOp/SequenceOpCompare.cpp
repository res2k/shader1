#include "intermediate/SequenceOp/SequenceOpCompare.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCompare::SequenceOpCompare (RegisterID destination,
					  SequenceVisitor::CompareOp op,
					  RegisterID source1,
					  RegisterID source2)
     : SequenceOpBinary (destination, source1, source2), op (op)
    {
    }
    
    void SequenceOpCompare::Visit (SequenceVisitor& visitor)
    {
      visitor.OpCompare (destination, op, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
