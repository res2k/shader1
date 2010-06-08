#include "intermediate/SequenceOp/SequenceOpUnaryInv.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpUnaryInv::SequenceOpUnaryInv (RegisterID destination,
					    RegisterID source)
     : SequenceOpUnary (destination, source)
    {
    }
    
    void SequenceOpUnaryInv::Visit (SequenceVisitor& visitor)
    {
      visitor.OpUnaryInv (destination, source);
    }
        
  } // namespace intermediate
} // namespace s1
