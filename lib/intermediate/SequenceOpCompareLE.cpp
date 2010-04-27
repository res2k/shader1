#include "intermediate/SequenceOpCompareLE.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCompareLE::SequenceOpCompareLE (Sequence::RegisterID destination,
					      Sequence::RegisterID source1,
					      Sequence::RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpCompareLE::Visit (SequenceVisitor& visitor)
    {
      visitor.OpCompareLE (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
