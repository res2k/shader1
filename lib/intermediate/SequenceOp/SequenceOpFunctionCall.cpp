#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpFunctionCall::SequenceOpFunctionCall (const RegisterID& destination,
						    const UnicodeString& funcIdent,
						    const std::vector<RegisterID>& inParams,
						    const std::vector<RegisterID>& outParams)
     : SequenceOpWithResult (destination), funcIdent (funcIdent), inParams (inParams), outParams (outParams)
    {
    }
    
    RegisterIDSet SequenceOpFunctionCall::GetReadRegisters () const
    {
      RegisterIDSet regs;
      regs.insert (inParams.begin(), inParams.end());
      return regs;
    }
    
    RegisterIDSet SequenceOpFunctionCall::GetWrittenRegisters () const
    {
      RegisterIDSet regs (SequenceOpFunctionCall::GetWrittenRegisters());
      regs.insert (outParams.begin(), outParams.end());
      return regs;
    }
    
    void SequenceOpFunctionCall::Visit (SequenceVisitor& visitor)
    {
      visitor.OpFunctionCall (destination, funcIdent, inParams, outParams);
    }
  } // namespace intermediate
} // namespace s1
