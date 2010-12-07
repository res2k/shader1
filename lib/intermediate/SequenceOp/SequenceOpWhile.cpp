#include "base/common.h"
#include "intermediate/SequenceOp/SequenceOpWhile.h"
#include "intermediate/SequenceVisitor.h"

#include <boost/foreach.hpp>

namespace s1
{
  namespace intermediate
  {
    SequenceOpWhile::SequenceOpWhile (const RegisterPtr& conditionReg,
				      const std::vector<std::pair<RegisterPtr, RegisterPtr> >& loopedRegs,
				      const SequenceOpPtr& bodyOp)
     : conditionReg (conditionReg), loopedRegs (loopedRegs), bodyOp (bodyOp)
    {
    }
    
    RegisterSet SequenceOpWhile::GetReadRegisters () const
    {
      RegisterSet regs (bodyOp->GetReadRegisters());
      BOOST_FOREACH(const LoopedRegsPair& loopedReg, loopedRegs)
      {
	regs.insert (loopedReg.first);
      }
      return regs;
    }
    
    RegisterSet SequenceOpWhile::GetWrittenRegisters () const
    {
      RegisterSet regs (bodyOp->GetWrittenRegisters());
      BOOST_FOREACH(const LoopedRegsPair& loopedReg, loopedRegs)
      {
	regs.insert (loopedReg.second);
      }
      return regs;
    }
    
    void SequenceOpWhile::Visit (SequenceVisitor& visitor)
    {
      visitor.OpWhile (conditionReg, loopedRegs, bodyOp);
    }
  } // namespace intermediate
} // namespace s1
