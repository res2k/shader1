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
