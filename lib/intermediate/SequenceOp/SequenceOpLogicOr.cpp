#include "intermediate/SequenceOp/SequenceOpLogicOr.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpLogicOr::SequenceOpLogicOr (RegisterID destination,
					  RegisterID source1,
					  RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpLogicOr::Visit (SequenceVisitor& visitor)
    {
      visitor.OpLogicOr (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
