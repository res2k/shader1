#ifndef __INTERMEDIATE_SEQUENCEOPWHILE_H__
#define __INTERMEDIATE_SEQUENCEOPWHILE_H__

#include "SequenceOp.h"
#include "../forwarddecl.h"
#include "../RegisterID.h"

#include <vector>

namespace s1
{
  namespace intermediate
  {
    class SequenceOpWhile : public SequenceOp
    {
      RegisterID conditionReg;
      std::vector<std::pair<RegisterID, RegisterID> > loopedRegs;
      SequenceOpPtr bodyOp;
    public:
      SequenceOpWhile (RegisterID conditionReg,
		       const std::vector<std::pair<RegisterID, RegisterID> >& loopedRegs,
		       const SequenceOpPtr& bodyOp);
      
      RegisterIDSet GetReadRegisters () const;
      RegisterIDSet GetWrittenRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPWHILE_H__
