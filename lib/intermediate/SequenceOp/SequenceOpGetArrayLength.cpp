#include "intermediate/SequenceOp/SequenceOpGetArrayLength.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpGetArrayLength::SequenceOpGetArrayLength (RegisterID destination,
							RegisterID source)
     : SequenceOpUnary (destination, source)
    {}

    void SequenceOpGetArrayLength::Visit (SequenceVisitor& visitor)
    {
      visitor.OpGetArrayLength (destination, source);
    }
    
  } // namespace intermediate
} // namespace s1
