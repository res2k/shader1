/*
    Shader1
    Copyright (c) 2010-2014 Frank Richter


    This library is free software; you can redistribute it and/or
    modify it under the terms of the wxWindows Library Licence.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Please see the wxWindows Library Licence for more details.
    The licence texts can be found in the source distribution files
    LICENCE-wxWindows.txt and LICENCE-LGPL.txt.
*/

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
