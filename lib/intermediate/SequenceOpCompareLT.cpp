#include "intermediate/SequenceOpCompareLT.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCompareLT::SequenceOpCompareLT (Sequence::RegisterID destination,
					      Sequence::RegisterID source1,
					      Sequence::RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpCompareLT::Visit (SequenceVisitor& visitor)
    {
      visitor.OpCompareLT (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
