#include "intermediate/SequenceOp/SequenceOpAssign.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpAssign::SequenceOpAssign (Sequence::RegisterID destination,
				        Sequence::RegisterID source)
     : SequenceOpUnary (destination, source)
    {
    }
    
    void SequenceOpAssign::Visit (SequenceVisitor& visitor)
    {
      visitor.OpAssign (destination, source);
    }
  } // namespace intermediate
} // namespace s1
