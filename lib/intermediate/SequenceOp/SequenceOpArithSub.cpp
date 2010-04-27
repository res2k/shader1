#include "intermediate/SequenceOp/SequenceOpArithSub.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpArithSub::SequenceOpArithSub (Sequence::RegisterID destination,
					    Sequence::RegisterID source1,
					    Sequence::RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpArithSub::Visit (SequenceVisitor& visitor)
    {
      visitor.OpArithSub (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
