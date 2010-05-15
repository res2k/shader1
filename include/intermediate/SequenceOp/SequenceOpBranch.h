#ifndef __INTERMEDIATE_SEQUENCEOPBRANCH_H__
#define __INTERMEDIATE_SEQUENCEOPBRANCH_H__

#include "SequenceOp.h"
#include "../forwarddecl.h"
#include "../RegisterID.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpBranch : public SequenceOp
    {
      RegisterID conditionReg;
      SequenceOpPtr trueOp;
      SequenceOpPtr falseOp;
    public:
      SequenceOpBranch (RegisterID conditionReg,
			const SequenceOpPtr& trueOp,
			const SequenceOpPtr& falseOp);
      
      RegisterIDSet GetReadRegisters () const;
      RegisterIDSet GetWrittenRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPBRANCH_H__
