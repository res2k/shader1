#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpCast.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCast::SequenceOpCast (RegisterPtr destination,
				    BasicType castTo,
				    RegisterPtr source)
     : SequenceOpUnary (destination, source), castTo (castTo)
    {
    }
    
    void SequenceOpCast::Visit (SequenceVisitor& visitor)
    {
      visitor.OpCast (destination, castTo, source);
    }
  } // namespace intermediate
} // namespace s1
