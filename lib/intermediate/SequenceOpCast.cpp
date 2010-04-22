#include "intermediate/SequenceOpCast.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpCast::SequenceOpCast (Sequence::RegisterID destination,
				    BasicType castTo,
				    Sequence::RegisterID source)
     : SequenceOpUnary (destination, source), castTo (castTo)
    {
    }
    
  } // namespace intermediate
} // namespace s1
