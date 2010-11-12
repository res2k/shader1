#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpReturn.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpReturn::SequenceOpReturn (const std::vector<RegisterPtr>& outParamVals)
     : outParamVals (outParamVals)
    {
    }
    
    RegisterSet SequenceOpReturn::GetReadRegisters () const
    {
      RegisterSet regs;
      regs.insert (outParamVals.begin(), outParamVals.end());
      return regs;
    }
    
    RegisterSet SequenceOpReturn::GetWrittenRegisters () const
    {
      RegisterSet regs;
      return regs;
    }
    
    void SequenceOpReturn::Visit (SequenceVisitor& visitor)
    {
      visitor.OpReturn (outParamVals);
    }
  } // namespace intermediate
} // namespace s1
