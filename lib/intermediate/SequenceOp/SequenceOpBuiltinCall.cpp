#include "intermediate/SequenceOp/SequenceOpBuiltinCall.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBuiltinCall::SequenceOpBuiltinCall (const RegisterID& destination,
						  BuiltinFunction what,
						  const std::vector<RegisterID>& inParams)
     : SequenceOpWithResult (destination), what (what), inParams (inParams)
    {
    }
    
    RegisterIDSet SequenceOpBuiltinCall::GetReadRegisters () const
    {
      RegisterIDSet regs;
      regs.insert (inParams.begin(), inParams.end());
      return regs;
    }
    
    void SequenceOpBuiltinCall::Visit (SequenceVisitor& visitor)
    {
      visitor.OpBuiltinCall (destination, what, inParams);
    }
  } // namespace intermediate
} // namespace s1
