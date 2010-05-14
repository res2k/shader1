#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBlock::SequenceOpBlock (const SequencePtr& subSequence,
				      const Sequence::IdentifierToRegIDMap& identToRegIDs)
     : subSequence (subSequence), identToRegIDs (identToRegIDs)
    {
    }
    
    void SequenceOpBlock::Visit (SequenceVisitor& visitor)
    {
      visitor.OpBlock (subSequence, identToRegIDs);
    }
  } // namespace intermediate
} // namespace s1
