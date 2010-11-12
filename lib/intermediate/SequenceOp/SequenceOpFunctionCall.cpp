#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpFunctionCall::SequenceOpFunctionCall (const UnicodeString& funcIdent,
						    const std::vector<RegisterPtr>& inParams,
						    const std::vector<RegisterPtr>& outParams)
     : funcIdent (funcIdent), inParams (inParams), outParams (outParams)
    {
    }
    
    RegisterSet SequenceOpFunctionCall::GetReadRegisters () const
    {
      RegisterSet regs;
      regs.insert (inParams.begin(), inParams.end());
      return regs;
    }
    
    RegisterSet SequenceOpFunctionCall::GetWrittenRegisters () const
    {
      RegisterSet regs;
      regs.insert (outParams.begin(), outParams.end());
      return regs;
    }
    
    void SequenceOpFunctionCall::Visit (SequenceVisitor& visitor)
    {
      visitor.OpFunctionCall (funcIdent, inParams, outParams);
    }
  } // namespace intermediate
} // namespace s1
