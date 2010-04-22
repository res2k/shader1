#include "intermediate/SequenceOpAssign.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpAssign::SequenceOpAssign (Sequence::RegisterID destination,
				        Sequence::RegisterID source)
     : SequenceOpUnary (destination, source)
    {
    }
    
  } // namespace intermediate
} // namespace s1
