#include "intermediate/SequenceOp/SequenceOpUnaryNot.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpUnaryNot::SequenceOpUnaryNot (RegisterID destination,
					    RegisterID source)
     : SequenceOpUnary (destination, source)
    {
    }
    
    void SequenceOpUnaryNot::Visit (SequenceVisitor& visitor)
    {
      visitor.OpUnaryNot (destination, source);
    }
        
  } // namespace intermediate
} // namespace s1
