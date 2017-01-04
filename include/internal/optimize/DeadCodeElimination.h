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

#ifndef __DEADCODEELIMINATION_H__
#define __DEADCODEELIMINATION_H__

#include "intermediate/Sequence.h"

namespace s1
{
  namespace optimize
  {
    class DeadCodeElimination
    {
      class DeadCodeChecker;
    public:
      static bool EliminateDeadCode (const intermediate::SequenceBuilderPtr& outputSeq,
                                     const intermediate::SequencePtr& inputSeq,
                                     const intermediate::RegisterSet& usedRegistersSeed);
    };
  } // namespace optimize
} // namespace s1

#endif // __DEADCODEELIMINATION_H__
