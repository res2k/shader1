#include "intermediate/SequenceOp/SequenceOpCompareGE.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCompareGE::SequenceOpCompareGE (RegisterID destination,
					      RegisterID source1,
					      RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpCompareGE::Visit (SequenceVisitor& visitor)
    {
      visitor.OpCompareGE (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
