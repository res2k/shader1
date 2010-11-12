#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpBranch.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBranch::SequenceOpBranch (RegisterPtr conditionReg,
					const SequenceOpPtr& trueOp,
					const SequenceOpPtr& falseOp)
     : conditionReg (conditionReg), trueOp (trueOp), falseOp (falseOp)
    {
    }
    
    RegisterSet SequenceOpBranch::GetReadRegisters () const
    {
      RegisterSet regs;
      regs.insert (conditionReg);
      if (trueOp)
      {
	RegisterSet trueRegs (trueOp->GetReadRegisters());
	regs.insert (trueRegs.begin(), trueRegs.end());
      }
      if (falseOp)
      {
	RegisterSet falseRegs (falseOp->GetReadRegisters());
	regs.insert (falseRegs.begin(), falseRegs.end());
      }
      return regs;
    }
    
    RegisterSet SequenceOpBranch::GetWrittenRegisters () const
    {
      RegisterSet regs;
      if (trueOp)
      {
	RegisterSet trueRegs (trueOp->GetWrittenRegisters());
	regs.insert (trueRegs.begin(), trueRegs.end());
      }
      if (falseOp)
      {
	RegisterSet falseRegs (falseOp->GetWrittenRegisters());
	regs.insert (falseRegs.begin(), falseRegs.end());
      }
      return regs;
    }
    
    void SequenceOpBranch::Visit (SequenceVisitor& visitor)
    {
      visitor.OpBranch (conditionReg, trueOp, falseOp);
    }
  } // namespace intermediate
} // namespace s1
