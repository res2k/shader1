#include "intermediate/SequenceOp/SequenceOpReturn.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpReturn::SequenceOpReturn (const std::vector<RegisterID>& outParamVals)
     : outParamVals (outParamVals)
    {
    }
    
    RegisterIDSet SequenceOpReturn::GetReadRegisters () const
    {
      RegisterIDSet regs;
      regs.insert (outParamVals.begin(), outParamVals.end());
      return regs;
    }
    
    RegisterIDSet SequenceOpReturn::GetWrittenRegisters () const
    {
      RegisterIDSet regs;
      return regs;
    }
    
    void SequenceOpReturn::Visit (SequenceVisitor& visitor)
    {
      visitor.OpReturn (outParamVals);
    }
  } // namespace intermediate
} // namespace s1
