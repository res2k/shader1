#ifndef __INTERMEDIATE_SEQUENCEOPBRANCH_H__
#define __INTERMEDIATE_SEQUENCEOPBRANCH_H__

#include "SequenceOp.h"
#include "../forwarddecl.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpBranch : public SequenceOp
    {
      RegisterPtr conditionReg;
      SequenceOpPtr trueOp;
      SequenceOpPtr falseOp;
    public:
      SequenceOpBranch (RegisterPtr conditionReg,
			const SequenceOpPtr& trueOp,
			const SequenceOpPtr& falseOp);
      
      RegisterSet GetReadRegisters () const;
      RegisterSet GetWrittenRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPBRANCH_H__
