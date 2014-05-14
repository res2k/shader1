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

#ifndef __SEQUENCEOPTIMIZATIONS_H__
#define __SEQUENCEOPTIMIZATIONS_H__

#include "intermediate/forwarddecl.h"
#include "intermediate/Sequence.h"

#include <vector>

namespace s1
{
  namespace optimize
  {
    class SequenceOptimizations
    {
    public:
      static bool Apply (unsigned int optimizations,
			 intermediate::SequencePtr& outputSeq,
			 const intermediate::SequencePtr& inputSeq,
			 const intermediate::RegisterSet& usedRegs);
    };
  } // namespace optimize
} // namespace s1

#endif // __SEQUENCEOPTIMIZATIONS_H__
