#include "intermediate/SequenceOp/SequenceOpBranch.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBranch::SequenceOpBranch (RegisterID conditionReg,
					const SequenceOpPtr& trueOp,
					const SequenceOpPtr& falseOp)
     : conditionReg (conditionReg), trueOp (trueOp), falseOp (falseOp)
    {
    }
    
    RegisterIDSet SequenceOpBranch::GetReadRegisters () const
    {
      RegisterIDSet regs;
      regs.insert (conditionReg);
      if (trueOp)
      {
	RegisterIDSet trueRegs (trueOp->GetReadRegisters());
	regs.insert (trueRegs.begin(), trueRegs.end());
      }
      if (falseOp)
      {
	RegisterIDSet falseRegs (falseOp->GetReadRegisters());
	regs.insert (falseRegs.begin(), falseRegs.end());
      }
      return regs;
    }
    
    RegisterIDSet SequenceOpBranch::GetWrittenRegisters () const
    {
      RegisterIDSet regs;
      if (trueOp)
      {
	RegisterIDSet trueRegs (trueOp->GetWrittenRegisters());
	regs.insert (trueRegs.begin(), trueRegs.end());
      }
      if (falseOp)
      {
	RegisterIDSet falseRegs (falseOp->GetWrittenRegisters());
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
