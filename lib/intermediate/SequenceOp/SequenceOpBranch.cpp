/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

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
