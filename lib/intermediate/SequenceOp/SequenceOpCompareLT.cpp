#include "intermediate/SequenceOp/SequenceOpCompareLT.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCompareLT::SequenceOpCompareLT (RegisterID destination,
					      RegisterID source1,
					      RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpCompareLT::Visit (SequenceVisitor& visitor)
    {
      visitor.OpCompareLT (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
