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

#ifndef __OPTIMIZE_OPTIMIZESEQUENCEDEADCODEELIMINATE_H__
#define __OPTIMIZE_OPTIMIZESEQUENCEDEADCODEELIMINATE_H__

#include "OptimizeSequenceStep.h"

#include "intermediate/Sequence.h"
#include "optimize/Optimizer.h"

namespace s1
{
  namespace optimize
  {
    class OptimizeSequenceDeadCodeEliminate : public OptimizeSequenceStep
    {
      intermediate::RegisterSet usedRegistersSeed;
    public:
      OptimizeSequenceDeadCodeEliminate (const intermediate::RegisterSet& usedRegistersSeed);
      
      unsigned int Apply (const intermediate::SequenceBuilderPtr& outputSeq,
			  const intermediate::SequencePtr& inputSeq);
			  
      unsigned int FilterOptimizerFlags (unsigned int optimizers)
      { return optimizers & ~Optimizer::optDeadCodeElimination; }
    };
  } // namespace optimize
} // namespace s1

#endif // __OPTIMIZE_OPTIMIZESEQUENCEDEADCODEELIMINATE_H__
