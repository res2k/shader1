#include "intermediate/SequenceOp/SequenceOpArithDiv.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpArithDiv::SequenceOpArithDiv (RegisterID destination,
					    RegisterID source1,
					    RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpArithDiv::Visit (SequenceVisitor& visitor)
    {
      visitor.OpArithDiv (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
