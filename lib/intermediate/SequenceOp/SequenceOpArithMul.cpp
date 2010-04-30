#include "intermediate/SequenceOp/SequenceOpArithMul.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpArithMul::SequenceOpArithMul (RegisterID destination,
					    RegisterID source1,
					    RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpArithMul::Visit (SequenceVisitor& visitor)
    {
      visitor.OpArithMul (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
