#include "intermediate/SequenceOp/SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBinary::SequenceOpBinary (Sequence::RegisterID destination,
					Sequence::RegisterID source1,
					Sequence::RegisterID source2)
     : SequenceOpWithResult (destination), source1 (source1), source2 (source2)
    {
    }
    
  } // namespace intermediate
} // namespace s1
