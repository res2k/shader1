#include "intermediate/SequenceOp/SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBinary::SequenceOpBinary (RegisterID destination,
					RegisterID source1,
					RegisterID source2)
     : SequenceOpWithResult (destination), source1 (source1), source2 (source2)
    {
    }
    
    RegisterIDSet SequenceOpBinary::GetReadRegisters () const
    {
      RegisterIDSet regs;
      regs.insert (source1);
      regs.insert (source2);
      return regs;
    }
        
  } // namespace intermediate
} // namespace s1
