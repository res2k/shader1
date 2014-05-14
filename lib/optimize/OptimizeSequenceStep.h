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

#ifndef __OPTIMIZE_OPTIMIZESEQUENCESTEP_H__
#define __OPTIMIZE_OPTIMIZESEQUENCESTEP_H__

#include "intermediate/forwarddecl.h"

namespace s1
{
  namespace optimize
  {
    struct OptimizeSequenceStep
    {
      virtual ~OptimizeSequenceStep() {}
      
      /// Flags indicating how the optimize step changed the sequence
      enum
      {
	/**
	 * Some operations were expanded into multiple operations.
	 * Might give new opportunities for dead code removal or
	 * constant propagation.
	 */
	opsExpanded = 1 << 0,
	/**
	 * Some operations have been removed, the sets of read/written
	 * registers have probably changed.
	 */
	opsRemoved = 1 << 1,
	/// Some new nested block ops were added
	opsBlocksChanged = 1 << 2,
      };
      virtual unsigned int Apply (const intermediate::SequencePtr& outputSeq,
				  const intermediate::SequencePtr& inputSeq) = 0;
				  
      virtual unsigned int FilterOptimizerFlags (unsigned int optimizers) = 0;
    };
  } // namespace optimize
} // namespace s1

#endif // __OPTIMIZE_OPTIMIZESEQUENCESTEP_H__
