#include "intermediate/SequenceOp/SequenceOpReturn.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpReturn::SequenceOpReturn (RegisterID retValReg)
     : retValReg (retValReg)
    {
    }
    
    RegisterIDSet SequenceOpReturn::GetReadRegisters () const
    {
      RegisterIDSet regs;
      if (retValReg.IsValid())
	regs.insert (retValReg);
      return regs;
    }
    
    RegisterIDSet SequenceOpReturn::GetWrittenRegisters () const
    {
      RegisterIDSet regs;
      return regs;
    }
    
    void SequenceOpReturn::Visit (SequenceVisitor& visitor)
    {
      visitor.OpReturn (retValReg);
    }
  } // namespace intermediate
} // namespace s1
