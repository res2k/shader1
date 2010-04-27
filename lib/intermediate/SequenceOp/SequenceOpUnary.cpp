#include "intermediate/SequenceOp/SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpUnary::SequenceOpUnary (Sequence::RegisterID destination,
				      Sequence::RegisterID source)
     : SequenceOpWithResult (destination), source (source)
    {
    }
    
  } // namespace intermediate
} // namespace s1
