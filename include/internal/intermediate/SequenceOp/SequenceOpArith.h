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

#ifndef __INTERMEDIATE_SEQUENCEOPARITH_H__
#define __INTERMEDIATE_SEQUENCEOPARITH_H__

#include "../SequenceVisitor.h"
#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpArith : public SequenceOpBinary
    {
      SequenceVisitor::ArithmeticOp op;
    public:
      SequenceOpArith (RegisterPtr destination,
                       SequenceVisitor::ArithmeticOp op,
                       RegisterPtr source1,
                       RegisterPtr source2);

      void Visit (SequenceVisitor& visitor);
    };

  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPARITH_H__
