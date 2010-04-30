#include "intermediate/SequenceOp/SequenceOpCompareEq.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCompareEq::SequenceOpCompareEq (RegisterID destination,
					      RegisterID source1,
					      RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpCompareEq::Visit (SequenceVisitor& visitor)
    {
      visitor.OpCompareEq (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
