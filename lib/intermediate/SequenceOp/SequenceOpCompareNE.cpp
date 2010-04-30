#include "intermediate/SequenceOp/SequenceOpCompareNE.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCompareNE::SequenceOpCompareNE (RegisterID destination,
					      RegisterID source1,
					      RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpCompareNE::Visit (SequenceVisitor& visitor)
    {
      visitor.OpCompareNE (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
