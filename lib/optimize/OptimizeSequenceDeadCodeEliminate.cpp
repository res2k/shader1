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
#include "OptimizeSequenceDeadCodeEliminate.h"

#include "intermediate/SequenceBuilder.h"

#include "optimize/DeadCodeElimination.h"

namespace s1
{
  namespace optimize
  {
    OptimizeSequenceDeadCodeEliminate::OptimizeSequenceDeadCodeEliminate (const intermediate::RegisterSet& usedRegistersSeed)
     : usedRegistersSeed (usedRegistersSeed)
    {
    }
    
    unsigned int OptimizeSequenceDeadCodeEliminate::Apply (const intermediate::SequenceBuilderPtr& outputSeq,
							   const intermediate::SequencePtr& inputSeq)
    {
      bool seqChanged = DeadCodeElimination::EliminateDeadCode (outputSeq, inputSeq, usedRegistersSeed);
      
      if (seqChanged)
      {
	outputSeq->CleanUnusedImportsExports (usedRegistersSeed);
	return opsRemoved;
      }
      else
	return 0;
    }
  } // namespace optimize
} // namespace s1

