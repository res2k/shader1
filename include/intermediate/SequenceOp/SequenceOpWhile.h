#ifndef __INTERMEDIATE_SEQUENCEOPWHILE_H__
#define __INTERMEDIATE_SEQUENCEOPWHILE_H__

#include "SequenceOp.h"
#include "../forwarddecl.h"

#include <vector>

namespace s1
{
  namespace intermediate
  {
    class SequenceOpWhile : public SequenceOp
    {
      RegisterPtr conditionReg;
      std::vector<std::pair<RegisterPtr, RegisterPtr> > loopedRegs;
      SequenceOpPtr bodyOp;
    public:
      SequenceOpWhile (const RegisterPtr& conditionReg,
		       const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
		       const SequenceOpPtr& bodyOp);
      
      RegisterSet GetReadRegisters () const;
      RegisterSet GetWrittenRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPWHILE_H__
