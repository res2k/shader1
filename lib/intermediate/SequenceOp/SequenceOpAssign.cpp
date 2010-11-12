#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpAssign.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpAssign::SequenceOpAssign (RegisterPtr destination,
				        RegisterPtr source)
     : SequenceOpUnary (destination, source)
    {
    }
    
    void SequenceOpAssign::Visit (SequenceVisitor& visitor)
    {
      visitor.OpAssign (destination, source);
    }
  } // namespace intermediate
} // namespace s1
