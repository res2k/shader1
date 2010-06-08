#include "intermediate/SequenceOp/SequenceOpUnaryNeg.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpUnaryNeg::SequenceOpUnaryNeg (RegisterID destination,
					    RegisterID source)
     : SequenceOpUnary (destination, source)
    {
    }
    
    void SequenceOpUnaryNeg::Visit (SequenceVisitor& visitor)
    {
      visitor.OpUnaryNeg (destination, source);
    }
        
  } // namespace intermediate
} // namespace s1
