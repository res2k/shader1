#include "intermediate/SequenceOp/SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpUnary::SequenceOpUnary (RegisterID destination,
				      RegisterID source)
     : SequenceOpWithResult (destination), source (source)
    {
    }
    
  } // namespace intermediate
} // namespace s1
