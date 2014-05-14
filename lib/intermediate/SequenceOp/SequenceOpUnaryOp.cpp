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
#include "intermediate/SequenceOp/SequenceOpUnaryOp.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpUnaryOp::SequenceOpUnaryOp (RegisterPtr destination,
					  SequenceVisitor::UnaryOp op,
					  RegisterPtr source)
     : SequenceOpUnary (destination, source), op (op)
    {
    }
    
    void SequenceOpUnaryOp::Visit (SequenceVisitor& visitor)
    {
      visitor.OpUnary (destination, op, source);
    }
  } // namespace intermediate
} // namespace s1
