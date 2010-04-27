#include "intermediate/SequenceOpArithAdd.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpArithAdd::SequenceOpArithAdd (Sequence::RegisterID destination,
					    Sequence::RegisterID source1,
					    Sequence::RegisterID source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }
    
    void SequenceOpArithAdd::Visit (SequenceVisitor& visitor)
    {
      visitor.OpArithAdd (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
