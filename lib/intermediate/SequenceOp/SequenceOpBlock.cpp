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
#include "intermediate/SequenceOp/SequenceOpBlock.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpBlock::SequenceOpBlock (const SequencePtr& subSequence,
				      const Sequence::IdentifierToRegMap& identToRegs_imp,
				      const Sequence::IdentifierToRegMap& identToRegs_exp)
     : subSequence (subSequence),
       identToRegs_imp (identToRegs_imp),
       identToRegs_exp (identToRegs_exp),
       readRegisters (subSequence->GetImportOuterRegs (identToRegs_imp)),
       writtenRegisters (subSequence->GetExportOuterRegs (identToRegs_exp))
    {
    }
    
    RegisterSet SequenceOpBlock::GetReadRegisters () const
    {
      RegisterSet regs;
      regs.insert (readRegisters.begin(), readRegisters.end());
      return regs;
    }
    
    RegisterSet SequenceOpBlock::GetWrittenRegisters () const
    {
      RegisterSet regs;
      regs.insert (writtenRegisters.begin(), writtenRegisters.end());
      return regs;
    }
    
    void SequenceOpBlock::Visit (SequenceVisitor& visitor)
    {
      visitor.OpBlock (subSequence, identToRegs_imp, identToRegs_exp);
    }
  } // namespace intermediate
} // namespace s1
