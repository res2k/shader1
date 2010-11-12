#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpBuiltinCall.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBuiltinCall::SequenceOpBuiltinCall (const RegisterPtr& destination,
						  BuiltinFunction what,
						  const std::vector<RegisterPtr>& inParams)
     : SequenceOpWithResult (destination), what (what), inParams (inParams)
    {
    }
    
    RegisterSet SequenceOpBuiltinCall::GetReadRegisters () const
    {
      RegisterSet regs;
      regs.insert (inParams.begin(), inParams.end());
      return regs;
    }
    
    void SequenceOpBuiltinCall::Visit (SequenceVisitor& visitor)
    {
      visitor.OpBuiltinCall (destination, what, inParams);
    }
  } // namespace intermediate
} // namespace s1
