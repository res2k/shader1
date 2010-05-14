#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBlock::SequenceOpBlock (const SequencePtr& subSequence,
				      const Sequence::IdentifierToRegIDMap& identToRegIDs,
				      const std::vector<RegisterID>& writtenRegisters)
     : subSequence (subSequence), identToRegIDs (identToRegIDs), writtenRegisters (writtenRegisters)
    {
    }
    
    void SequenceOpBlock::Visit (SequenceVisitor& visitor)
    {
      visitor.OpBlock (subSequence, identToRegIDs, writtenRegisters);
    }
  } // namespace intermediate
} // namespace s1
