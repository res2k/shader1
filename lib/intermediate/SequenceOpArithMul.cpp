#include "intermediate/SequenceOpArithMul.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpArithMul::SequenceOpArithMul (Sequence::RegisterID destination,
					    Sequence::RegisterID source1,
					    Sequence::RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpArithMul::Visit (SequenceVisitor& visitor)
    {
      visitor.OpArithMul (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
