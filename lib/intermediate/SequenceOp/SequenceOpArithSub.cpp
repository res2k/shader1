#include "intermediate/SequenceOp/SequenceOpArithSub.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpArithSub::SequenceOpArithSub (RegisterID destination,
					    RegisterID source1,
					    RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpArithSub::Visit (SequenceVisitor& visitor)
    {
      visitor.OpArithSub (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
