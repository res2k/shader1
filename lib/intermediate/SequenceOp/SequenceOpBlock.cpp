#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBlock::SequenceOpBlock (const SequencePtr& subSequence,
				      const Sequence::IdentifierToRegIDMap& identToRegIDs_imp,
				      const Sequence::IdentifierToRegIDMap& identToRegIDs_exp,
				      const std::vector<RegisterID>& writtenRegisters)
     : subSequence (subSequence), identToRegIDs_imp (identToRegIDs_imp),
       identToRegIDs_exp (identToRegIDs_exp), writtenRegisters (writtenRegisters)
    {
    }
    
    RegisterIDSet SequenceOpBlock::GetReadRegisters () const
    {
      RegisterIDSet regs;
      /* FIXME: Probably have to record the 'read' registers in the sequence op as well */
      return regs;
    }
    
    RegisterIDSet SequenceOpBlock::GetWrittenRegisters () const
    {
      RegisterIDSet regs;
      regs.insert (writtenRegisters.begin(), writtenRegisters.end());
      return regs;
    }
    
    void SequenceOpBlock::Visit (SequenceVisitor& visitor)
    {
      visitor.OpBlock (subSequence, identToRegIDs_imp, identToRegIDs_exp, writtenRegisters);
    }
  } // namespace intermediate
} // namespace s1
