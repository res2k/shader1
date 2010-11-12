#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpWithResult.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpWithResult::SequenceOpWithResult (RegisterPtr destination)
     : destination (destination)
    {
    }
    
    RegisterSet SequenceOpWithResult::GetWrittenRegisters () const
    {
      RegisterSet regs;
      regs.insert (destination);
      return regs;
    }
    
  } // namespace intermediate
} // namespace s1
