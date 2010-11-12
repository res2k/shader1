#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBinary::SequenceOpBinary (RegisterPtr destination,
					RegisterPtr source1,
					RegisterPtr source2)
     : SequenceOpWithResult (destination), source1 (source1), source2 (source2)
    {
    }
    
    RegisterSet SequenceOpBinary::GetReadRegisters () const
    {
      RegisterSet regs;
      regs.insert (source1);
      regs.insert (source2);
      return regs;
    }
        
  } // namespace intermediate
} // namespace s1
