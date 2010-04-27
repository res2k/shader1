#include "intermediate/SequenceOp/SequenceOpCompareGT.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCompareGT::SequenceOpCompareGT (Sequence::RegisterID destination,
					      Sequence::RegisterID source1,
					      Sequence::RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpCompareGT::Visit (SequenceVisitor& visitor)
    {
      visitor.OpCompareGT (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
