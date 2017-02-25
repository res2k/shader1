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

#ifndef __INTERMEDIATE_SEQUENCEOPVECTORDOT_H__
#define __INTERMEDIATE_SEQUENCEOPVECTORDOT_H__

#include "../forwarddecl.h"
#include "SequenceOpBinary.h"

namespace s1
{
  namespace intermediate
  {
    class SequenceOpVectorDot : public SequenceOpBinary
    {
    public:
      SequenceOpVectorDot (RegisterPtr destination,
                           RegisterPtr source1,
                           RegisterPtr source2);

      void Visit (SequenceVisitor& visitor);
    };
  } // namespace intermediate
} // namespace s1

#endif // __INTERMEDIATE_SEQUENCEOPVECTORDOT_H__
