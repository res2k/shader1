#include "intermediate/SequenceOp/SequenceOpLogicAnd.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpLogicAnd::SequenceOpLogicAnd (RegisterID destination,
					    RegisterID source1,
					    RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpLogicAnd::Visit (SequenceVisitor& visitor)
    {
      visitor.OpLogicAnd (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
