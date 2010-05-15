#include "intermediate/SequenceOp/SequenceOpUnary.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpUnary::SequenceOpUnary (RegisterID destination,
				      RegisterID source)
     : SequenceOpWithResult (destination), source (source)
    {
    }
    
    RegisterIDSet SequenceOpUnary::GetReadRegisters () const
    {
      RegisterIDSet regs;
      regs.insert (source);
      return regs;
    }
        
  } // namespace intermediate
} // namespace s1
