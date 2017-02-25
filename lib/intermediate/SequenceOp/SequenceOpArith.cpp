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
#include "intermediate/SequenceOp/SequenceOpArith.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpArith::SequenceOpArith (RegisterPtr destination,
                                      SequenceVisitor::ArithmeticOp op,
                                      RegisterPtr source1,
                                      RegisterPtr source2)
     : SequenceOpBinary (destination, source1, source2), op (op)
    {
    }

    void SequenceOpArith::Visit (SequenceVisitor& visitor)
    {
      visitor.OpArith (destination, op, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
