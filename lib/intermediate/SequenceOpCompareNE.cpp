#include "intermediate/SequenceOpCompareNE.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCompareNE::SequenceOpCompareNE (Sequence::RegisterID destination,
					      Sequence::RegisterID source1,
					      Sequence::RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpCompareNE::Visit (SequenceVisitor& visitor)
    {
      visitor.OpCompareNE (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
