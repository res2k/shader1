#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpUnary::SequenceOpUnary (RegisterPtr destination,
				      RegisterPtr source)
     : SequenceOpWithResult (destination), source (source)
    {
    }
    
    RegisterSet SequenceOpUnary::GetReadRegisters () const
    {
      RegisterSet regs;
      regs.insert (source);
      return regs;
    }
        
  } // namespace intermediate
} // namespace s1
