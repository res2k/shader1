#include "intermediate/SequenceOp/SequenceOpWithResult.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpWithResult::SequenceOpWithResult (RegisterID destination)
     : destination (destination)
    {
    }
    
    RegisterIDSet SequenceOpWithResult::GetWrittenRegisters () const
    {
      RegisterIDSet regs;
      regs.insert (destination);
      return regs;
    }
    
  } // namespace intermediate
} // namespace s1
