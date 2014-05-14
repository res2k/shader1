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
#include "intermediate/SequenceOp/SequenceOpFunctionCall.h"
#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpFunctionCall::SequenceOpFunctionCall (const UnicodeString& funcIdent,
						    const std::vector<RegisterPtr>& inParams,
						    const std::vector<RegisterPtr>& outParams)
     : funcIdent (funcIdent), inParams (inParams), outParams (outParams)
    {
    }
    
    RegisterSet SequenceOpFunctionCall::GetReadRegisters () const
    {
      RegisterSet regs;
      regs.insert (inParams.begin(), inParams.end());
      return regs;
    }
    
    RegisterSet SequenceOpFunctionCall::GetWrittenRegisters () const
    {
      RegisterSet regs;
      regs.insert (outParams.begin(), outParams.end());
      return regs;
    }
    
    void SequenceOpFunctionCall::Visit (SequenceVisitor& visitor)
    {
      visitor.OpFunctionCall (funcIdent, inParams, outParams);
    }
  } // namespace intermediate
} // namespace s1
