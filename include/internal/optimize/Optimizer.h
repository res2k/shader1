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

#ifndef __OPTIMIZE_OPTIMIZER_H__
#define __OPTIMIZE_OPTIMIZER_H__

#include "intermediate/forwarddecl.h"

namespace s1
{
  namespace optimize
  {
    class Optimizer
    {
      unsigned int optimizations;
      void SetOptFlag (unsigned int flag, bool val)
      {
	if (val)
	  optimizations |= flag;
	else
	  optimizations &= ~flag;
      }
    public:
      enum
      {
	optInlineBlocks = 1 << 0,
	optDeadCodeElimination = 1 << 1,
	optConstantFolding = 1 << 2,
        optUnusedArgumentRemoval = 1 << 3
      };
      Optimizer() : optimizations (0) {}
      
      void SetInlineBlocks (bool flag) { SetOptFlag (optInlineBlocks, flag); }
      void SetDeadCodeElimination (bool flag) { SetOptFlag (optDeadCodeElimination, flag); }
      void SetConstantFolding (bool flag) { SetOptFlag (optConstantFolding, flag); }
      void SetUnusedArgumentRemoval (bool flag) { SetOptFlag (optUnusedArgumentRemoval, flag); }
      
      intermediate::ProgramPtr ApplyOptimizations (const intermediate::ProgramPtr& program);
    };
  } // namespace optimize
} // namespace s1

#endif // __OPTIMIZE_OPTIMIZER_H__
