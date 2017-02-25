/*
    Shader1
    Copyright (c) 2017 Frank Richter


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
#include "intermediate/SequenceOp/SequenceOpVectorCross.h"

#include "intermediate/SequenceVisitor.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpVectorCross::SequenceOpVectorCross (RegisterPtr destination,
                                                  RegisterPtr source1,
                                                  RegisterPtr source2)
     : SequenceOpBinary (destination, source1, source2)
    {
    }

    void SequenceOpVectorCross::Visit (SequenceVisitor& visitor)
    {
      visitor.OpVectorCross (destination, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
