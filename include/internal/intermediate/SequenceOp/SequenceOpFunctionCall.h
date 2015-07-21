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

#ifndef __INTERMEDIATE_SEQUENCEOPFUNCTIONCALL_H__
#define __INTERMEDIATE_SEQUENCEOPFUNCTIONCALL_H__

#include "SequenceOp.h"
#include "../forwarddecl.h"

#include <vector>

namespace s1
{
  namespace intermediate
  {
    class SequenceOpFunctionCall : public SequenceOp
    {
      uc::String funcIdent;
      std::vector<RegisterPtr> inParams;
      std::vector<RegisterPtr> outParams;
    public:
      SequenceOpFunctionCall (const uc::String& funcIdent,
			      const std::vector<RegisterPtr>& inParams,
			      const std::vector<RegisterPtr>& outParams);
      
      RegisterSet GetReadRegisters () const;
      RegisterSet GetWrittenRegisters () const;
      
      void Visit (SequenceVisitor& visitor);
    };
    
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPFUNCTIONCALL_H__
