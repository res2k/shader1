#include "intermediate/SequenceOp/SequenceOpCompareLE.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCompareLE::SequenceOpCompareLE (RegisterID destination,
					      RegisterID source1,
					      RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpCompareLE::Visit (SequenceVisitor& visitor)
    {
      visitor.OpCompareLE (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1