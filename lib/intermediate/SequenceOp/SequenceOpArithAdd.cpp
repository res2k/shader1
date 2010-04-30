#include "intermediate/SequenceOp/SequenceOpArithAdd.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpArithAdd::SequenceOpArithAdd (RegisterID destination,
					    RegisterID source1,
					    RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpArithAdd::Visit (SequenceVisitor& visitor)
    {
      visitor.OpArithAdd (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
