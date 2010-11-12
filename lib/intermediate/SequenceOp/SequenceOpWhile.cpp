#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpWhile.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpWhile::SequenceOpWhile (const RegisterPtr& conditionReg,
				      const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
				      const SequenceOpPtr& bodyOp)
     : conditionReg (conditionReg), loopedRegs (loopedRegs), bodyOp (bodyOp)
    {
    }
    
    RegisterSet SequenceOpWhile::GetReadRegisters () const
    {
      RegisterSet regs;
      /*regs.insert (conditionReg);
      if (trueOp)
      {
	RegisterIDSet trueRegs (trueOp->GetReadRegisters());
	regs.insert (trueRegs.begin(), trueRegs.end());
      }
      if (falseOp)
      {
	RegisterIDSet falseRegs (falseOp->GetReadRegisters());
	regs.insert (falseRegs.begin(), falseRegs.end());
      }*/
      return regs;
    }
    
    RegisterSet SequenceOpWhile::GetWrittenRegisters () const
    {
      RegisterSet regs;
      /*if (trueOp)
      {
	RegisterIDSet trueRegs (trueOp->GetWrittenRegisters());
	regs.insert (trueRegs.begin(), trueRegs.end());
      }
      if (falseOp)
      {
	RegisterIDSet falseRegs (falseOp->GetWrittenRegisters());
	regs.insert (falseRegs.begin(), falseRegs.end());
      }*/
      return regs;
    }
    
    void SequenceOpWhile::Visit (SequenceVisitor& visitor)
    {
      visitor.OpWhile (conditionReg, loopedRegs, bodyOp);
    }
  } // namespace intermediate
} // namespace s1
