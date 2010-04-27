#include "intermediate/SequenceOpArithMod.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpArithMod::SequenceOpArithMod (Sequence::RegisterID destination,
					    Sequence::RegisterID source1,
					    Sequence::RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpArithMod::Visit (SequenceVisitor& visitor)
    {
      visitor.OpArithMod (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
