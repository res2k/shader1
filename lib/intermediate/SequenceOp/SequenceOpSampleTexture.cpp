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
#include "intermediate/SequenceOp/SequenceOpSampleTexture.h"

namespace s1
{
  namespace intermediate
  {
    SequenceOpSampleTexture::SequenceOpSampleTexture (RegisterPtr destination,
                                                      SequenceVisitor::SampleTextureOp op,
                                                      RegisterPtr sampler,
                                                      RegisterPtr coord)
     : SequenceOpBinary (destination, sampler, coord), op (op)
    {
    }

    void SequenceOpSampleTexture::Visit (SequenceVisitor& visitor)
    {
      visitor.OpSampleTexture (destination, op, source1, source2);
    }
  } // namespace intermediate
} // namespace s1
