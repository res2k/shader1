#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBlock::SequenceOpBlock (const SequencePtr& subSequence,
				      const Sequence::IdentifierToRegMap& identToRegs_imp,
				      const Sequence::IdentifierToRegMap& identToRegs_exp,
				      const std::vector<RegisterPtr>& readRegisters,
				      const std::vector<RegisterPtr>& writtenRegisters)
     : subSequence (subSequence), identToRegs_imp (identToRegs_imp),
       identToRegs_exp (identToRegs_exp), readRegisters (readRegisters),
       writtenRegisters (writtenRegisters)
    {
    }
    
    RegisterSet SequenceOpBlock::GetReadRegisters () const
    {
      RegisterSet regs;
      regs.insert (readRegisters.begin(), readRegisters.end());
      return regs;
    }
    
    RegisterSet SequenceOpBlock::GetWrittenRegisters () const
    {
      RegisterSet regs;
      regs.insert (writtenRegisters.begin(), writtenRegisters.end());
      return regs;
    }
    
    void SequenceOpBlock::Visit (SequenceVisitor& visitor)
    {
      visitor.OpBlock (subSequence, identToRegs_imp, identToRegs_exp, writtenRegisters);
    }
  } // namespace intermediate
} // namespace s1
